#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from functools import partial
import os
import subprocess
import sys
import time

import yama
from yama import comint


DEFAULT_ADDR = ('localhost', 2222)


def deploy(cmd):
    print('Deploying...')
    subprocess.check_output(cmd, shell=True, stderr=subprocess.STDOUT)
    print('Deploying done')


def mel_source(name):
    if not name.endswith('.mel'):
        name += '.mel'
    fn = os.path.join(yama.__path__[0], name)
    # XXX: What about quotes in fn?
    return 'source "{}";'.format(fn.replace('\\', '/'))


def mel_prep(arg):
    if isinstance(arg, str):
        return '"{}"'.format(arg)
    return arg


def mel_fn(fn, *args):
    return '{}({});'.format(fn, ', '.join(map(mel_prep, args)))


def unload(scene, plugins):
    source = mel_source('unload')
    fn = mel_fn('ydmUnload', scene, plugins)
    return ' '.join((source, fn))


def load(scene, plugins):
    source = mel_source('load')
    fn = mel_fn('ydmLoad', scene, plugins)
    return ' '.join((source, fn))


def unique_scene_name():
    return '{}.mb'.format(time.time())


def main():
    scene = unique_scene_name()
    plugins = sys.argv[1]
    cmd = sys.argv[2]

    com = comint.MayaComint(partial(print, end=''))
    try:
        com.connect(DEFAULT_ADDR)
    # except ConnectionRefusedError:
    except:
        print('Connection refused: addr={}'.format(DEFAULT_ADDR))
        deploy(cmd)
    else:
        # 1. Unload
        com.write(unload(scene, plugins))
        time.sleep(3)

        # 2. Deploy
        deploy(cmd)

        # 3. Load
        com.write(load(scene, plugins))

        # 4. Print output and close
        com.wait()
        time.sleep(3)
        com.close()


if __name__ == '__main__':
    main()
