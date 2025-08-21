
import streamlit as st
import cv2
import tempfile
import numpy as np
from gtts import gTTS
from ultralytics import YOLO
import os

st.title("🔍 YOLOv8 Object Detection + 🎙 Voice Output")
st.write("Upload an image or use webcam for real-time detection.")

model = YOLO("yolov8n.pt")

# Function to detect and speak
def detect_and_speak(frame):
    results = model(frame)
    detected = set()
    for r in results:
        for c in r.boxes.cls:
            detected.add(model.names[int(c)])
    label_text = ", ".join(detected)
    if label_text:
        st.success(f"Detected: {label_text}")
        tts = gTTS(text=f"Detected {label_text}", lang="en")
        tts.save("detected.mp3")
        audio_file = open("detected.mp3", "rb")
        st.audio(audio_file.read(), format="audio/mp3")
        audio_file.close()
    return results[0].plot()

# Webcam input
if st.button("Use Webcam"):
    cap = cv2.VideoCapture(0)
    stframe = st.empty()
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
        result_img = detect_and_speak(frame)
        stframe.image(result_img, channels="BGR")
    cap.release()

# Image upload
uploaded_file = st.file_uploader("Upload an image", type=["jpg", "jpeg", "png"])
if uploaded_file is not None:
    file_bytes = np.asarray(bytearray(uploaded_file.read()), dtype=np.uint8)
    image = cv2.imdecode(file_bytes, 1)
    result_img = detect_and_speak(image)
    st.image(result_img, caption="Detected Objects", channels="BGR")
