#!/usr/bin/env bash

cd /home/tobias/Playground/grow-active/grow_active_server
source .venv/bin/activate
gunicorn -c scripts/gunicorn.conf.py grow_active_server.wsgi:application
