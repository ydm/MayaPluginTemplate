#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# Command interpreter (comint) for Maya.
#
# If started as __main__, it serves two purposes:
#
# 1. If there are not any command line arguments, it starts a MEL REPL.
#
# 2. If there are command line arguments, it writes them to Maya one by
# one in the same order.
#

from __future__ import print_function
import collections
import socket
import sys
import threading
from functools import partial


DEFAULT_ADDR = ('localhost', 2222)


class MayaReader(object):

    def __init__(self):
        super(MayaReader, self).__init__()
        self._chunks = collections.deque()

    def clear(self, replace=None):
        if replace is not None:
            self._chunks = replace
        else:
            self._chunks = []

    def read(self, skt):
        chunks = []
        while 1:
            timeout = skt.gettimeout()
            skt.settimeout(0.1)  # Or 0.0 if non-blocking
            try:
                chunk = skt.recv(4096)
            # except BlockingIOError:
            except socket.timeout:
                break
            else:
                chunks.append(chunk)
            finally:
                skt.settimeout(timeout)
        return self._process_chunks(chunks)

    def _process_chunks(self, chunks):
        for chunk in chunks:
            self._chunks.append(chunk)
        ans = []
        while len(self._chunks) > 0:
            chunk = self._chunks.popleft()
            idx = chunk.find(b'\n\0')
            if idx != -1:
                head = chunk[:idx]
                tail = chunk[idx + 2:]
                head and ans.append(head)
                tail and self._chunks.appendleft(tail)
        return b''.join(ans)


def writemsg(skt, msg, size):
    totalsent = 0
    while totalsent < size:
        sent = skt.send(msg[totalsent:])
        if sent == 0:
            raise RuntimeError('socket connection broken')
        totalsent += sent
    return totalsent


class MayaComint(object):

    def __init__(self, cb=None):
        super(MayaComint, self).__init__()
        # self._input_cb = cb or (lambda s: None)
        self._input_cb = cb
        self._read_event = threading.Event()
        self._reader = MayaReader()
        self._run = True
        self._socket = socket.socket()
        self._thread = threading.Thread(target=self.loop, args=(self._socket,))

    def connect(self, addr):
        self._socket.connect(addr)
        self._thread.start()

    def close(self):
        self._run = False
        self._thread.join()
        self._socket.close()

    def loop(self, skt):
        while self._run:
            line = self._reader.read(skt)
            decoded = line.decode('utf-8')
            decoded and self._input_cb(decoded)
            self._read_event.set()

    def wait(self, timeout=None):
        return self._read_event.wait(timeout)

    def write(self, text):
        binary = text.encode(sys.getdefaultencoding())
        self._read_event.clear()
        writemsg(self._socket, binary, len(binary))


def check_encoding():
    enc = sys.getdefaultencoding()
    if enc != 'utf-8':
        print('Warning: default encoding is {}, expected utf-8'.format(enc))


def repl():
    comint = MayaComint(partial(print))
    try:
        comint.connect(DEFAULT_ADDR)
    except ConnectionRefusedError:
        print('Connection refused: addr={}'.format(DEFAULT_ADDR))
        return
    while 1:
        inp = input('')
        if inp == 'exit':
            break
        comint.write(inp)
        comint.wait()
    comint.close()


def write_lines(lines):
    comint = MayaComint(partial(print, end=''))
    try:
        comint.connect(DEFAULT_ADDR)
    except ConnectionRefusedError:
        print('Connection refused: addr={}'.format(DEFAULT_ADDR))
        return
    for line in lines:
        comint.write(line)
        comint.wait()
    comint.close()


def main():
    check_encoding()
    if len(sys.argv) == 1:
        repl()
    else:
        write_lines(sys.argv[1:])


if __name__ == '__main__':
    main()
