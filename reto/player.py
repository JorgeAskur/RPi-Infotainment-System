from pytube import Search
import urllib.request
import pygame.mixer
import subprocess
import ffmpeg
import random
import serial
import glob
import csv

from tkinter import *
from tkinter import filedialog, Label, PhotoImage
from PIL import Image
import tkinter

serialSTM=serial.Serial ("/dev/ttyUSB0", 115200)

#Iniciar la ventana
ventana = Tk()
ventana.title("GUI")
ventana.geometry("800x800")
ventana.minsize(800, 800)
ventana.maxsize(2560, 1600)
ventana.configure(bg= 'black')
bgventana = tkinter.PhotoImage(file="space.png")
lblventana = Label(ventana, image = bgventana).place(x=0, y=00)

#Iniciar mixer
pygame.mixer.init()

#Lista de diccionarios con la info de cada cancion
music=[] #music[0]['author']

def csvRead():
    global music
    f = open("songs.csv", "r")
    reader = csv.DictReader(f)
    music = list(reader)
    f.close()

def csvWrite():
    global music
    keys = music[0].keys()

    f = open("songs.csv", "w")
    writer = csv.DictWriter(f, keys)
    writer.writeheader()
    writer.writerows(music)
    f.close()


data_cancion = ''
data_select = 0

tk_cancion = StringVar()
tk_select = StringVar()
lbl_Lista = Label(text="Lista de opciones:", font=("Roboto Cn",8))
lbl_Lista['bg'] = '#D95DF5'
lbl_Lista.place(x=644,y=250, height=50, width=140)

lblLista = Text(ventana, font=("Roboto Cn",8))
lblLista['bg'] = '#D95DF5'
lblLista.place(x=644,y=275, height=150, width=155)

cancion_entry = Entry(textvariable=tk_cancion)
cancion_entry.place(x=644,y=200, height=25, width=155)

select_entry = Entry(textvariable=tk_select)
select_entry.place(x=644,y=425, height=25, width=155)

videoList = []
def songSearch():
    global music
    global videoList
    global data_cancion
    global data_select
    global lblLista
    lblLista.delete('1.0', END)
    #El usuario pone la cancion que quiera       
    data_cancion = tk_cancion.get()
        
    #Pytube la busca
    s = Search(data_cancion)


    #Se imprime una lista con las opciones
    i = 0
    videoList = s.results
    for video in videoList:
      lblLista.insert(END, str(i) + ") " + video.title + "\n" )
      i = i + 1
    print("\n")
        
       
#Funcion para descargar canciones
def DownloadSong():
    global music
    global data_select
    global data_cancion
    global videoList
    data_select = str(tk_select.get())
    data_cancion = str(tk_cancion.get())
       
    data_select = int(data_select)
    print(videoList[data_select].title)

    #Descarga de la cancion
    print("Iniciando descarga ... ")
    #yt = videoList[index]
    yt = videoList[data_select]
        
    #Descarga de la imagen de la cancion
    imgLink = yt.thumbnail_url
    #urllib.request.urlretrieve(imgLink, (query + ".jpg"))
    urllib.request.urlretrieve(imgLink, (data_cancion + ".jpg"))
    
    #Convertir a .png
    #img = Image.open(query + ".jpg")
    img = Image.open(data_cancion + ".jpg")
    img = img.resize((300,200), Image.ANTIALIAS)
    #img.save(query + ".png")
    img.save(data_cancion + ".png")
    
    #jpg = query + ".jpg"
    jpg = data_cancion + ".jpg"

    #Descarga de la cancion en mp4
    stream = yt.streams.filter(only_audio=True).get_audio_only()
    #stream.download(filename = query + ".mp4")
    stream.download(filename = data_cancion + ".mp4")
    
    #Conversion a mp3
    #filename = query + ".mp4"
    filename = data_cancion + ".mp4"
    mp4 =ffmpeg.input(filename)
    audio = mp4.audio
    #stream = ffmpeg.output(audio, query + ".mp3")
    stream = ffmpeg.output(audio, data_cancion + ".mp3")
    ffmpeg.run(stream)
    print("Fin de la descarga")
    
    #Eliminar archivo mp4
    subprocess.run(["rm", filename])
    subprocess.run(["rm", jpg])

    #Creacion del diccionario
    songData = {
        'name'   : data_cancion,
        'author' : yt.author,
        'length' : yt.length,
        'image'  : data_cancion + ".png"
    }

    music.append(songData)
        
    #Mostrar la nueva lista de canciones
    music = sorted(music, key = lambda i: i['name'])
    print("Nueva lista de canciones")
    load()
    csvWrite()
    #print(songs)
    #print(music)
    lblLista.delete('1.0', END)

#Carga de canciones inicial
def sload():
    global songs
    songs = []
    i = 0

    #Carga de canciones
    for file in glob.glob("*.mp3"):
        songs.append(file)
        i = i + 1
    songs.sort()

    global imgs
    imgs = []
    i = 0

    #Carga de imagenes
    for file in glob.glob("*.png"):
        imgs.append(file)
        i = i + 1
    imgs.sort()
    csvRead()

#Carga de canciones e imprime las canciones cargadas
def load():
    global songs
    songs = []
    i = 0

    for file in glob.glob("*.mp3"):
        songs.append(file)
        i = i + 1
    songs.sort()
    
    global imgs
    imgs = []
    i = 0

    #Carga de imagenes
    for file in glob.glob("*.png"):
        imgs.append(file)
        i = i + 1
    imgs.sort()

lblLenght = 0 
lblnombre = ''
lblautor = '' 
lblimage = ''
cont_play = 0
minutos = 0
segundos = 0
lbl_duracion = '00:00'
minutos2 = 0
segundos2 = 0
cont_play2 = '00:00'
#Funcion que reproduce la cancion segun el indice dado
def play(song):
    global songs
    global lblLenght
    global lblnombre
    global lblautor 
    global lblimage
    global minutos
    global segundos
    global minutos2
    global segundos2
    global cont_play
    global cont_play2
    global lbl_duracion
    pygame.mixer.music.load(songs[song])
    pygame.mixer.music.play()
    lblLenght = music[song]['length']
    lblnombre = music[song]['name']
    lblautor = music[song]['author']
    lblimage = music[song]['image']
    lblName.configure(text="Nombre: " + lblnombre)
    lblArtista.configure(text="Artista: " + lblautor)
    imagenew=tkinter.PhotoImage(file=lblimage)
    lblDuracion.configure(text= cont_play2 + " --------------------------------------- " + lbl_duracion)
    
    lblAlbum.configure(image=imagenew)
    lblAlbum.image = imagenew
    serialTx()
    
    
def time_pp():
    global minutos
    global segundos
    global minutos2
    global current
    global segundos2
    global cont_play
    global cont_play2
    global lbl_duracion
    global lblLenght
    lblLenght = int(lblLenght)
    minutos = lblLenght//60
    segundos = lblLenght % 60
    lbl_duracion = str(minutos)+":"+str(segundos)
    cont_play = current
    minutos2 = cont_play//60
    segundos2 = cont_play % 60
    cont_play2 = str(minutos2)+":"+str(segundos2)
    lblDuracion.configure(text= cont_play2 + " --------------------------------------- " + lbl_duracion)

def lista():
    global songs
    global lblLista
    lblLista.delete('1.0', END)
    i = 0
    for item in songs:
        lblLista.insert(END, str(i) + ") " + str(item) + "\n" )
        i = i + 1

def select():
    global playing
    global song
    global data_select
    global songs
    global lblLista
    lblLista.delete('1.0', END)
    data_select = int(tk_select.get())
    #Se cargan las opciones
    load()
    #El usuario selecciona la cancion deseada
    #Se reproduce y se actualiza la variable de seguimiento
    song = data_select
    play(song)
    playing = True
    
    lblLista.delete('1.0', END)

#Pausa
def playPause():
    global playing
    #Si algo se esta reproduciendo se pausa
    if playing:
        pygame.mixer.music.pause()
        playing = False
    #Si no se quita la pausa
    else:
        pygame.mixer.music.unpause()
        playing = True

#Avanza a la siguiente cancion
def forward():
    global song
    global songs
    global playing
    song = song + 1
    if song > (len(songs)-1):
        song = 0

    play(song)
    playing = True

#Retrocede a la cancion anterior
def backward():
    global song
    global songs 
    global playing
    song = song - 1
    if song < 0:
        song = len(songs) - 1

    play(song)
    playing = True

#Reinicia la cancion actual
def rewind():
    global current
    global song
    #pygame.mixer.music.rewind()
    play(song)
    current = 0
    time_pp()

def randomSong():
    global song
    global songs
    global playing
    #Generar un numero aleatorio entre 0 y el indice maximo de la lista de canciones y reproducir esa cancion
    song = random.randint(0, (len(songs)-1))
    play(song)
    playing = True

volume = 50
pygame.mixer.music.set_volume(volume)

#Ajustar volumen
def volumeUp():
    global volume

    if volume != 100:
        volume = volume + 10

    volume2 = volume / 100
    pygame.mixer.music.set_volume(volume2)

def volumeDown():
    global volume

    if volume != 0:
        volume = volume - 10

    volume2 = volume / 100
    pygame.mixer.music.set_volume(volume2)

def serialRe():
    global entrada
    if serialSTM.isOpen() and serialSTM.in_waiting:
        #decodifcar el dato
        entrada=serialSTM.readline().decode('ascii').rstrip('\n')
        entrada=ord(entrada)

def serialTx():
    global music
    global song
    
    global minutos
    global segundos
    global minutos2
    global current
    global segundos2
    global cont_play
    global cont_play2
    global lbl_duracion
    global lblLenght
    
    lblLenght = int(lblLenght)
    minutos = lblLenght//60
    segundos = lblLenght % 60
    lbl_duracion = str(minutos)+str(segundos)
    
        
    
    
    name=music[song]['name']
    if len(name)>16:
        name=name[0:15]
    length=lbl_duracion.rstrip(":")
    music[song]['length']
    artist=music[song]['author']
    if len(artist)>16:
        artist=artist[0:15]
        
        
    message=name+","+artist+","+lbl_duracion
    print (message)
    serialSTM.write(message.encode('ascii'))

#-----------------------------------------BOTON REPRODUCIR/PAUSAR--------------------------------------------
btnReproducir = Button(ventana, text="> / ||", font=("Roboto Cn",12), command=playPause)
btnReproducir['bg'] = '#92E528'
btnReproducir.place(x=425,y=400, height=50, width=63)

#-----------------------------------------BOTON BACKWARD--------------------------------------------
btnBack = Button(ventana, text="|<<", font=("Roboto Cn",12), command=backward)
btnBack['bg'] = '#D95DF5'
btnBack.place(x=362,y=400, height=50, width=63)

#-----------------------------------------BOTON FORWARD--------------------------------------------
btnFor = Button(ventana, text=">>|", font=("Roboto Cn",12), command=forward)
btnFor['bg'] = '#D95DF5'
btnFor.place(x=487,y=400, height=50, width=63)

#-----------------------------------------BOTON REWIND--------------------------------------------
btnFor = Button(ventana, text="<<", font=("Roboto Cn",12), command=rewind)
btnFor['bg'] = '#D95DF5'
btnFor.place(x=393,y=450, height=50, width=63)

#-----------------------------------------BOTON RANDOM--------------------------------------------
btnFor = Button(ventana, text="RANDOM", font=("Roboto Cn",8), command=randomSong)
btnFor['bg'] = '#D95DF5'
btnFor.place(x=456,y=450, height=50, width=63)

#-----------------------------------------LABEL VOLUMEN--------------------------------------------
lblVolumen = Label(ventana, text="VOLUMEN : " + str(volume), font=("Roboto Cn",8))
lblVolumen['bg'] = '#D95DF5'
lblVolumen.place(x=362,y=525, height=25, width=187)

#-----------------------------------------BOTON SUBIR VOLUMEN--------------------------------------------
btnvolup = Button(ventana, text=" + ", font=("Roboto Cn",12), command=volumeUp)
btnvolup['bg'] = '#D95DF5'
btnvolup.place(x=362,y=550, height=25, width=93)

#-----------------------------------------BOTON BAJAR VOLUMEN--------------------------------------------
btnvoldo = Button(ventana, text=" - ", font=("Roboto Cn",12), command=volumeDown)
btnvoldo['bg'] = '#D95DF5'
btnvoldo.place(x=456,y=550, height=25, width=93)

#-----------------------------------------BOTON DESCARGAR--------------------------------------------
btnDescargar = Button(ventana, text="DESCARGAR", font=("Roboto Cn",8), command=songSearch)
btnDescargar['bg'] = '#D95DF5'
btnDescargar.place(x=643,y=225, height=50, width=78)

#-----------------------------------------BOTON SELECCIONAR --------------------------------------------
btnselect_entry = Button(ventana, text="SELECCIONAR", font=("Roboto Cn",8), command=lista)
btnselect_entry['bg'] = '#D95DF5'
btnselect_entry.place(x=721,y=225, height=50, width=78)

#-----------------------------------------TEXTO BARRA DURACION--------------------------------------------
lblDuracion = Label(ventana, text= "00:00 --------------------------------------- 00:00", font=("Roboto Cn",6))
lblDuracion['bg'] = '#D95DF5'
lblDuracion.place(x=362,y=325, height=50, width=187)

#-----------------------------------------TEXTO NAME--------------------------------------------
lblName = Label(ventana, text= "Nombre: " + lblnombre, font=("Roboto Cn",10))
lblName['bg'] = '#D95DF5'
lblName.place(x=362,y=225, height=50, width=187)

#-----------------------------------------TEXTO ARTISTA--------------------------------------------
lblArtista = Label(ventana, text= "Artista: " + lblautor, font=("Roboto Cn",10))
lblArtista['bg'] = '#D95DF5'
lblArtista.place(x=362,y=275, height=50, width=187)

#-----------------------------------------IMAGEN ALBUM--------------------------------------------
album = tkinter.PhotoImage(file="intel_logo.png")
lblAlbum = Label(ventana, image = album)
lblAlbum['bg'] = '#000000'
lblAlbum.place(x=0,y=300, height=200, width=300)

#-----------------------------------------BOTON ENTER--------------------------------------------
btnselect_entry = Button(ventana, text="ENTER", font=("Roboto Cn",8), command=DownloadSong)
btnselect_entry['bg'] = '#D95DF5'
btnselect_entry.place(x=643,y=450, height=25, width=46)

#-----------------------------------------BOTON SELECT ENTER--------------------------------------------
btnselect_entry = Button(ventana, text="SELECT", font=("Roboto Cn",8), command=select)
btnselect_entry['bg'] = '#D95DF5'
btnselect_entry.place(x=752,y=450, height=25, width=46)

#Variable para saber si se esta reproduciendo algo
playing = False

#Variable que guarda el indice de la cancion actual
song = 0

#Carga inicial de canciones
sload()

current = 0
entrada = 0


while True:

    lblVolumen.configure(text="VOLUMEN : " + str(volume))
    
    current = round(pygame.mixer.music.get_pos() / 1000)
    if current >= int(music[song]["length"]):
        forward()
    time_pp()
    serialRe()
    
    if entrada == 1:
        playPause()

    #Avanzar
    elif entrada == 2:
        forward()

    #Retroceder
    elif entrada == 3:
        backward()

    entrada = 0


    ventana.update()