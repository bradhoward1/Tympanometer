# server.py

from flask import Flask, request, jsonify
from datetime import datetime
from pymodm import connect, MongoModel, fields
import PIL

connect("mongodb+srv://brad_howard:@cluster0-lucsp.mongodb.net"
        "/Tympanometer?retryWrites=true&w=majority")


app = Flask(__name__)


class SendData(MongoModel):
    subject = fields.CharField(primary_key=True)
    values = fields.ListField()


def __init__():
    print("Server is on.")


def check_keys(in_dict):
	my_keys = list(in_dict.keys())
	return my_keys


if __name__ == '__main__':
    __init__()
    app.run()
