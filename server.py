# server.py

from flask import Flask, request, jsonify
from datetime import datetime
from pymodm import connect, MongoModel, fields, errors
import PIL
import matplotlib.pyplot as plt
import csv
from flask_mail import Mail, Message
import requests

connect("mongodb+srv://<username>:<password>@cluster0-lucsp.mongodb.net"
        "/Tympanometer?retryWrites=true&w=majority")


app = Flask(__name__)
app.config.update(
    DEBUG=True,
    # EMAIL SETTINGS
    MAIL_SERVER='smtp.gmail.com',
    MAIL_PORT=465,
    MAIL_USE_SSL=True,
    MAIL_USERNAME='your@gmail.com',
    MAIL_PASSWORD='yourpassword'
    )
mail = Mail(app)


class SendData(MongoModel):
    subject = fields.CharField(primary_key=True)
    values = fields.ListField()
    pressure_values = fields.ListField()


def __init__():
    print("Server is on.")


def check_keys(in_dict):
    my_keys = list(in_dict.keys())
    return my_keys


def add_data(in_dict):
    new_info = SendData()
    keys = check_keys(in_dict)
    try:
        presence_check = SendData.objects.get({"_id": in_dict["subject"]})
    except SendData.DoesNotExist:
        presence_check = False
    if presence_check is False:
        add_new_vals(in_dict)
    else:
        add_vals(in_dict)
    return True


def add_vals(in_dict):
    new_info = SendData.objects.raw({"_id": in_dict["subject"]})
    new_info.update({"$push": {"values": in_dict["value_1"]}})
    new_info.update({"$push": {"values": in_dict["value_2"]}})
    new_info.update({"$push": {"values": in_dict["value_3"]}})
    new_info.update({"$push": {"values": in_dict["value_4"]}})
    new_info.update({"$push": {"values": in_dict["value_5"]}})
    new_info.update({"$push": {"values": in_dict["value_6"]}})
    new_info.update({"$push": {"values": in_dict["value_7"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_8"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_9"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_10"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_11"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_12"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_13"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_14"]}})


def add_new_vals(in_dict):
    new_info = SendData()
    new_info.subject = in_dict["subject"]
    new_info.values = [in_dict["value_1"]]
    new_info.pressure_values = [in_dict["value_8"]]
    new_info.save()
    new_info = SendData.objects.raw({"_id": in_dict["subject"]})
    new_info.update({"$push": {"values": in_dict["value_2"]}})
    new_info.update({"$push": {"values": in_dict["value_3"]}})
    new_info.update({"$push": {"values": in_dict["value_4"]}})
    new_info.update({"$push": {"values": in_dict["value_5"]}})
    new_info.update({"$push": {"values": in_dict["value_6"]}})
    new_info.update({"$push": {"values": in_dict["value_7"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_9"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_10"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_11"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_12"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_13"]}})
    new_info.update({"$push": {"pressure_values": in_dict["value_14"]}})


@app.route("/api/add_data", methods=["POST"])
def post_info():
    in_dict = request.get_json()
    add_data(in_dict)
    return "Good post made to server", 200


def patient_info(in_dict):
    # in_dict is going to look like this: {"name": "Name_Needed_For_ID"}
    patient_id = in_dict["data"].split("\"")
    patient_id = patient_id[2]
    patient_id = patient_id.replace(" ", "")
    patient_id = patient_id.replace("}", "")
    patient_id = patient_id.replace(":", "")
    patient = SendData.objects.raw({"_id": patient_id}).first()
    patient_mic_list = patient.values
    patient_pressure_list = patient.pressure_values
    return patient_mic_list, patient_pressure_list


@app.route("/api/get_mic_data", methods=["POST"])
def get_patient_info():
    in_dict = request.get_json()
    values, p_values = patient_info(in_dict)
    if values:
        create_csv(values, p_values)
        return jsonify(values), 200
    else:
        return "Unable to retrieve list of recorded mic values", 400


def create_csv(values, p_values):
    fields = ["Mic_Values", "Pressure_Values"]
    filename = "patient_info.csv"
    # writing to csv file
    with open(filename, 'w') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(fields)
        for value, p_value in zip(values, p_values):
            my_string = [str(value), str(p_value)]
            csvwriter.writerow(my_string)


def create_email(in_dict):
    txt = "{} Data"
    try:
        msg = Message(txt.format(in_dict["subject"]),
                      sender="bradleyhoward98@gmail.com",
                      recipients=["bradleyhoward98@gmail.com"])
        msg.body = '''
                   Attached below are the results for {}.
                   '''.format(in_dict["subject"])
        with app.open_resource("patient_info.csv") as fp:
            msg.attach("patient_info.csv", "patient_info/csv", fp.read())
        mail.send(msg)
        return 'Mail sent!'
    except:
        return "Something went wrong!"


@app.route("/api/send_email", methods=["POST"])
def send_email():
    in_dict = request.get_json()
    return_message = create_email(in_dict)
    return jsonify(return_message), 200


# def add_pressure_data(in_dict):
#     new_info = SendData()
#     keys = check_keys(in_dict)
#     try:
#         presence_check = SendData.objects.get({"_id": in_dict["subject"]})
#     except SendData.DoesNotExist:
#         presence_check = False
#     if presence_check is False:
#         add_new_pressure_vals(in_dict)
#     else:
#         add_pressure_vals(in_dict)
#     return True
#
#
# def add_pressure_vals(in_dict):
#     new_info = SendData.objects.raw({"_id": in_dict["subject"]})
#     new_info.update({"$push": {"pressure_values": in_dict["value_1"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_2"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_3"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_4"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_5"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_6"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_7"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_8"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_9"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_10"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_11"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_12"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_13"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_14"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_15"]}})


# def add_new_pressure_vals(in_dict):
#     new_info = SendData()
#     new_info.subject = in_dict["subject"]
#     new_info.pressure_values = [in_dict["value_1"]]
#     new_info.save()
#     new_info = SendData.objects.raw({"_id": in_dict["subject"]})
#     new_info.update({"$push": {"pressure_values": in_dict["value_2"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_3"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_4"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_5"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_6"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_7"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_8"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_9"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_10"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_11"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_12"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_13"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_14"]}})
#     new_info.update({"$push": {"pressure_values": in_dict["value_15"]}})


# @app.route("/api/add_pressure", methods=["POST"])
# def post_pressure_info():
#     in_dict = request.get_json()
#     add_pressure_data(in_dict)
#     return "Good post made to server", 200


if __name__ == '__main__':
    __init__()
    app.run()
