# server.py

from flask import Flask, request, jsonify
from datetime import datetime
from pymodm import connect, MongoModel, fields
import PIL

connect("mongodb+srv://brad_howard:@cluster0-lucsp.mongodb.net"
        "/final_database?retryWrites=true&w=majority")


app = Flask(__name__)


class SendData(MongoModel):
    mr_number = fields.CharField(primary_key=True)
    values = fields.ListField()


def __init__():
    print("Server is on.")


