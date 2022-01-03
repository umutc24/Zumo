#! /usr/bin/env python3
import sys
sys.path.append('/Library/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages')
from ncclient import manager
from device_info import ios_xe1

m = manager.connect_ssh(host="sandbox-iosxe-recomm-1.cisco.com",
                    port=830,
                    username="admin",
                    password="C1sco12345",
                    hostkey_verify=False) as m
m.close_session
