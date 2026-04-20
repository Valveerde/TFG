import tkinter as tk
from PIL import Image, ImageTk

root = tk.Tk()
root.title("Aplicación GUI")
root.geometry("1000x600")

# Cargar y redimensionar imágenes a tamaños reales aproximados (escala 1:10, 1cm=10px, aumentado x2)
images = {}
# Servomotor: ~4cm x 3cm -> 40x30 px
img = Image.open("images/Servomotor.PNG")
img = img.resize((40, 30), Image.LANCZOS)
images['servo'] = ImageTk.PhotoImage(img)

# Infrarrojo: ~2cm x 1cm -> 20x10 px
img = Image.open("images/infrarrojo.PNG")
img = img.resize((20, 10), Image.LANCZOS)
images['ir'] = ImageTk.PhotoImage(img)

# Stepper Motor: ~4cm x 4cm -> 40x40 px
img = Image.open("images/Steppermotor.PNG")
img = img.resize((40, 40), Image.LANCZOS)
images['stepper'] = ImageTk.PhotoImage(img)

# Ultrasonico: ~9cm x 4cm -> 90x40 px
img = Image.open("images/Ultrasónico.PNG")
img = img.resize((90, 40), Image.LANCZOS)
images['ultra'] = ImageTk.PhotoImage(img)

# Fan Motor: ~8cm x 8cm -> 80x80 px (doble de grande, x2)
img = Image.open("images/Fanmotor.PNG")
img = img.resize((80, 80), Image.LANCZOS)
images['fan'] = ImageTk.PhotoImage(img)

# DHT11: ~3cm x 2.4cm -> 30x24 px
img = Image.open("images/Dht11.PNG")
img = img.resize((30, 24), Image.LANCZOS)
images['dht'] = ImageTk.PhotoImage(img)

canvas = tk.Canvas(root, width=1000, height=600, bg='white')
canvas.pack()

# Las líneas de separación de celdas han sido removidas

# Parcela 1: Servomotor en la esquina superior izquierda
servo_label = tk.Label(root, image=images['servo'], bg='white')
canvas.create_window(0, 0, window=servo_label, anchor='nw')

# Parcela 2: Infrarrojo lo más pegado posible a la primera casilla
ir_label = tk.Label(root, image=images['ir'], bg='white')
canvas.create_window(250, 0, window=ir_label, anchor='nw')

# Botón al lado del infrarrojo, más centrado
button1 = tk.Button(root, text="Botón", font=("Arial", 16))
canvas.create_window(310, 20, window=button1)

# Otro botón debajo
button2 = tk.Button(root, text="Botón", font=("Arial", 16))
canvas.create_window(310, 80, window=button2)

# Parcela 3: Stepper Motor en el centro izquierda (más a la derecha)
stepper_label = tk.Label(root, image=images['stepper'], bg='white')
canvas.create_window(540, 150, window=stepper_label, anchor='w')

# Dos botones uno al lado del otro arriba del motor
button3 = tk.Button(root, text="Botón", font=("Arial", 16))
canvas.create_window(560, 100, window=button3)

button4 = tk.Button(root, text="Botón", font=("Arial", 16))
canvas.create_window(660, 100, window=button4)

# Parcela 4: Ultrasonico
ultra_label = tk.Label(root, image=images['ultra'], bg='white')
canvas.create_window(850, 150, window=ultra_label)

# Parcela 5: Fan Motor en el medio
fan_label = tk.Label(root, image=images['fan'], bg='white')
canvas.create_window(125, 450, window=fan_label)

# DHT11 entre la celda 5 y la 1 (bajado un poco)
dht_label = tk.Label(root, image=images['dht'], bg='white')
canvas.create_window(0, 170, window=dht_label, anchor='w')

root.mainloop()