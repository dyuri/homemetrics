#!/bin/env python

import requests
import json
import re

HOSTS = {
    '192.168.253.165': {
        'name': 'sensor 1'
    },
    '192.168.253.208': {
        'name': 'sensor 2'
    }
}


def parseValue(svalue):
    svalue = svalue.strip().split(' ')[0].strip()

    try:
        value = int(svalue)
    except:
        try:
            value = float(svalue)
        except:
            value = 'unknown'

    return value


def toKey(skey):
    return re.sub(r'[^\w]+', '_', skey.lower().strip())


def collect(host):
    r = requests.get('http://'+host)
    measurementlist = [m.split(':') for m in r.text.split('\n')]
    measurements = {
        toKey(m[0]): parseValue(m[1]) for m in measurementlist if len(m) > 1
    }

    return measurements


if __name__ == '__main__':
    response = []

    for host in HOSTS:
        data = collect(host)
        data['name'] = HOSTS[host]['name']

        response.append(data)

    print(json.dumps(response, indent=2))
