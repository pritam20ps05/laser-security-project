# Importing the required modules
from flask import Flask, request
from flask_mail import Mail, Message

#Modules for image processing
import cv2
import numpy as np
import os


# Initializing the App
app = Flask(__name__)

# Setting required constants
SENDER_EMAIL = os.environ['SENDER_MAIL']
RECIPIENT_EMAIL = os.environ['RECIPIENT_EMAIL']
MESSAGE_BODY = "It seems that there is a visitor please kindly check your home security feeds."
MESSAGE_SUBJECT = "Intruder Alert"

# Configuring app for sending mails
app.config['MAIL_SERVER'] = 'smtp.gmail.com'
app.config['MAIL_PORT'] = '465'
app.config['MAIL_USERNAME'] = os.environ['SENDER_MAIL']
app.config['MAIL_PASSWORD'] = os.environ['SENDER_PASS']
app.config['MAIL_USE_TLS'] = False
app.config['MAIL_USE_SSL'] = True

# Initializing mail instance
mail = Mail(app)


#Save image code
def save_img(img):
    img_dir = "esp32_imgs"
    outfile = os.path.join(img_dir, "img.jpg")
    if not os.path.isdir(img_dir):
        os.mkdir(img_dir)
    cv2.imwrite(outfile, img)
    return outfile


@app.route('/notify',methods=['POST'])
def send_message():

        message = Message(MESSAGE_SUBJECT,sender=SENDER_EMAIL,recipients=[RECIPIENT_EMAIL])
        message.body = MESSAGE_BODY

        #This is the part where we're gonna get the image data and process dat shit
        received = request
        of = ""
        if received.files:
            print(received.files.keys())
            # convert string of image data to uint8
            file  = received.files['imageFile']
            nparr = np.frombuffer(file.read(), np.uint8)
            # decode image
            img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
            of = save_img(img)

            with app.open_resource(of) as fp:
                message.attach('intruder.jpg', "image/jpg", fp.read())
            mail.send(message)
            
            print("Message sent")

            return "Alert Sent with img"
        else:
            mail.send(message)
            return "Alert Sent"



if __name__ == "__main__":
    app.run(debug=False, port=80, host="0.0.0.0")