import socket
import sys
from win32api import keybd_event
from win32con import KEYEVENTF_KEYUP

PORT = 35001

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('0.0.0.0', PORT))

sock.listen(1)


def parse_keycode(code):
    return 65


def parse_data(data):
    data = ord(data)
    key = parse_keycode(data & 0x7f)
    # If first bit set represents key down event.
    # If unset, key up.
    if data >> 7:
        return (key, 0)
    else:
        return (key, 0, KEYEVENTF_KEYUP)


def replay(data):
    try:
        keybd_event(*parse_data(data))
    except:
        print "Failed to parse incoming packet."


if __name__ == "__main__":
    while True:
        print 'Listening on port {}.'.format(PORT)

        connection, client_address = sock.accept()
        print 'Connected: {}.'.format(client_address)
        try:
            while True:
                data = connection.recv(16) # TODO: read spec on what this means
                if data:
                    replay(data)
                else:
                    print 'Disconnected: ', client_address
                    break
        finally:
            connection.close()
