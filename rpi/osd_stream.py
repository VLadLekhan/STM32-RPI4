# Set YOUR_PC_IP to the IP of the PC that receives the stream.

import threading
import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst, GLib
from pymavlink import mavutil

Gst.init(None)

PIPELINE = (
    'libcamerasrc ! video/x-raw,width=1280,height=720,framerate=30/1,format=NV12 ! '
    'textoverlay name=osd valignment=top halignment=left font-desc="Sans 20" ! queue ! '
    'v4l2h264enc extra-controls="controls,repeat_sequence_header=1,video_bitrate=2000000" ! '
    'video/x-h264,level=(string)4 ! h264parse config-interval=-1 ! mpegtsmux ! '
    'udpsink host=YOUR_PC_IP port=5000'
)

pipeline = Gst.parse_launch(PIPELINE)
osd = pipeline.get_by_name('osd')


def read_mavlink():
    master = mavutil.mavlink_connection('/dev/serial0', baud=115200)
    while True:
        msg = master.recv_match(type='RAW_IMU', blocking=True)
        x = msg.xacc / 16384
        y = msg.yacc / 16384
        z = msg.zacc / 16384
        osd.set_property('text', f'X: {x:.2f} g  Y: {y:.2f} g  Z: {z:.2f} g')


threading.Thread(target=read_mavlink, daemon=True).start()

pipeline.set_state(Gst.State.PLAYING)
loop = GLib.MainLoop()
try:
    loop.run()
except KeyboardInterrupt:
    pass
pipeline.set_state(Gst.State.NULL)
