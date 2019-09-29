import pyrebase
from flask import *
import pickle
import numpy as np
import cv2
from keras.preprocessing.image import img_to_array
from keras import backend as K
from pathlib import Path
import os
from firebase import firebase
import schedule
import time
config = {
   "apiKey": "AIzaSyCGZ7txBaWC1xnp2-lOJf95u0utTszsotY",
    "authDomain": "minor-25e4c.firebaseapp.com",
    "databaseURL": "https://minor-25e4c.firebaseio.com",
    "projectId": "minor-25e4c",
    "storageBucket": "minor-25e4c.appspot.com",
    "messagingSenderId": "78214323805"
  };

app = Flask(__name__)
@app.route("/")
def processing():
    firebase1 = firebase.FirebaseApplication("https://minor-25e4c.firebaseio.com/")
    firebase2 = pyrebase.initialize_app(config)

    storage = firebase2.storage()
    # storage.child("myimage.jpg").put("virus.jpg")
    storage.child("myimage.jpg").download("D:\\flask_with_firebase\\", "myimage1.jpg")
    file_save = "D:\\flask_with_firebase\\myimage1.jpg"
    default_img_size = ((256, 256))
    image = cv2.imread(file_save)
    image = cv2.resize(image, default_img_size)
    image = img_to_array(image)
    np_image = np.array(image, dtype=np.float16) / 225.0
    np_photo = np_image.reshape(1, 256, 256, 3)

    # Loading CNN model in model1
    model1 = pickle.load(open('data/cnn_model.pkl', 'rb'))
    pred_value = model1.predict_classes(np_photo)
    K.clear_session()
    print(pred_value)
    # loading Labelbinarizer as labelTran

    labelTran = pickle.load(open('data/label_transform.pkl', 'rb'))
    label = labelTran.classes_[pred_value].tolist()
    print(label)
    #firebase1.post('/label',{'Disease':label})
    firebase1.put('label','Disease',label)
    return "successfully"

schedule.every(1).seconds.do(processing)

schedule.every().hour.do(processing)
schedule.every().day.at("10:30").do(processing)

while 1:
    schedule.run_pending()
    time.sleep(1)


app.run(debug=True)
