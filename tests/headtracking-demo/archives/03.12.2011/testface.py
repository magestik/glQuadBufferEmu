#!/usr/bin/python
# -*- coding: utf8 -*-
"""
Track la tête de l'utilisateur en faisant appel aux caractéristiques pseudo-haar
avec l'interface python de OpenCv. Concrètement entoure le visage d'un rectangle (virtuel)
et à partir de ce rectangle trouve le milieu des yeux.
Transmet ensuite ces coordonnées à une scène OpenGL grâce au protocole OSC (pyLiblo)
"""
import sys, cv, liblo, time

#Variables globales de configuration (conf.cfg):
screenW = screenH = port = threshold = webcamID = 0
haarfile = IP = ""
Debug = 0
showCamera = 1

# Parameters for haar detection
# From the API:
# The default parameters (scale_factor=2, min_neighbors=3, flags=0) are tuned 
# for accurate yet slow object detection. For a faster operation on real video 
# images the settings are: 
# scale_factor=1.2, min_neighbors=2, flags=cv.CV_HAAR_DO_CANNY_PRUNING, 
# min_size=<minimum possible face size

min_size = (20, 20)
image_scale = 3#plus le nombre est grand plus c'est rapide mais moins c'est fiable
haar_scale = 1.1#idem, très significatifs au niveau des perfs(plus c'est petit, meilleure est la détection mais plus lent est l'algo)
min_neighbors = 3
haar_flags = cv.CV_HAAR_FIND_BIGGEST_OBJECT | cv.CV_HAAR_DO_ROUGH_SEARCH#optimisation, efficace

newTimeDebug = newTime = 0.0
oldTimeThr = oldTimeDebug = time.time()

centreCercle = (0,0)
somme = cpt = 0.0
#Charge le fichier de config et initialise les variables adéquates
def initConfig():
	global screenW,screenH,port,threshold,webcamID,haarfile,IP,Debug,showCamera
	arg = {}
	try:
		config = open('conf.cfg', 'rb')
	except IOError, err:
		print "Le fichier de configuration [conf.cfg] n'a pas été trouvé."
		print str(err)
		sys.exit(1)
	lignes = config.readlines()
	
	for lig in lignes:
		sp = lig.split('#')[0]
		sp = sp.split('=')
		if len(sp) == 2:
			arg[sp[0].strip()]=sp[1].strip()
	config.close()

	screenW = float(arg['screenwidth'])
	screenH = float(arg['screenheight'])
	IP = str(arg['ip'])
	port = int(arg['port'])
	threshold = int(arg['threshold'])
	haarfile = str(arg['haarfile'])
	webcamID = int(arg['webcamID'])
	Debug = int(arg['Debug'])
	showCamera = int(arg['showCamera'])
		
#détecte le visage dans une image et transmet ses coordonnées dans le référentiel 3D 
#ayant pour origine le centre de l'écran
def detect_and_send(img, cascade):
	global screenW, screenH, threshold, Debug,showCamera
	global oldTimeDebug, centreCercle
	global somme,cpt

	# allocate temporary images
	gray = cv.CreateImage((img.width,img.height), 8, 1)
	small_img = cv.CreateImage((cv.Round(img.width / image_scale),
	      cv.Round (img.height / image_scale)), 8, 1)
	
	# l'algo de reconnaissance de forme est basé sur des images en niveaux de gris
	cv.CvtColor(img, gray, cv.CV_BGR2GRAY)
	
	# Réduit la taille de l'image pour un traitement plus rapide
	cv.Resize(gray, small_img, cv.CV_INTER_LINEAR)
	
	#Augmente le contraste (égalise la luminosité)
	cv.EqualizeHist(small_img, small_img)

	if(cascade):
	
		t = cv.GetTickCount()
		faces = cv.HaarDetectObjects(small_img, cascade, cv.CreateMemStorage(0), \
	                             haar_scale, min_neighbors, haar_flags, min_size)
		t = cv.GetTickCount() - t
		cpt += 1
		somme += t/(cv.GetTickFrequency()*1000.)
		if Debug == 1:
			print "detection time = %gms" % (t/(cv.GetTickFrequency()*1000.))
		oldx = centreCercle[0]
		oldy = centreCercle[1]
		
		if faces:
			#oldx = centreCercle[0]
			#oldy = centreCercle[1]
			
			for ((x, y, w, h), n) in faces:
				#la bounding box encadrant le visage, sachant que l'image a été remdimensionnée pour le traitement (+ rapide)
				pt1 = (int(x * image_scale), int(y * image_scale))
				pt2 = (int((x + w) * image_scale), int((y + h) * image_scale))
				
				#pointe entre les deux yeux
				centreCercle = (pt1[0]+w*image_scale/2, pt1[1]+h*image_scale/3)
				cv.Circle(img,centreCercle, 2, cv.RGB(255,0,0))
				
			#On doit calculer l'opposé de la valeur qu'on trouverait car la caméra n'agit pas comme un miroir
			#en effet si je bouge vers la droite l'image fait bouger la tête vers la gauche (donc vers la droite selon mon
			#point de vue)
			ex = - (float(centreCercle[0]) /float(img.width) * screenW - screenW/2.0) 
			ey = - ((float(centreCercle[1]) / float(img.height) * screenH - screenH/2.0)) #+ screenH / 2.0 #compense le fait que la caméra est au dessus de l'écran

			#Détermination de ez en fonction de la taille de la tête à l'écran
			ez =  -1.50567*(h*image_scale) + 893.688
			
			#assure la stabilité de l'image, si on enlève ce test ça tremblote à mort car la détection n'étant pas ultra précise
			#la position de la tête est considérée comme changeante en permanence
			#la constante a été déterminée empiriquement sur une webcam intégrée de mcbook 13,3"(voir fichier conf.cfg)
			#Gestion du temps pour permettre de bouger lentement (fonctionne mal pour l'instant)
			newTime = time.time()
			if abs(oldx - centreCercle[0]) > threshold or abs(oldy - centreCercle[1]) > threshold :#or (newTime - oldTimeThr) > 1		
				liblo.send(target, "/coordonnees/", ex,ey,ez)
				oldTimeThr = newTime
				
			#DEBUG, affiche la position des yeux toutes les 1/2 secondes
			if Debug == 1:
				if (newTime - oldTimeDebug) > 0.5:
					print "\n-----------DEBUG----------"
					print "Coordonnée centre sur image : " + str(centreCercle[0]) +" "+ str(centreCercle[1])
					print "Coordonnées yeux dans référentiel 3D : "
					print "ex = " + str(ex) + " ey = " + str(ey) + " ez = " +str(ez)
					oldTimeDebug = newTime
				
	#debug tracking visage
	if showCamera == 1:
		cv.ShowImage("result", img)

if __name__ == '__main__':
	
	initConfig()
	
	print "Connexion au serveur"
	try:
		if IP == "localhost":
			adresse = port
		else:
			adresse = "osc.udp://" + IP + ":" + str(port)
		target = liblo.Address(adresse)
	except liblo.AddressError, err:
		print "Impossible de se connecter au serveur"
		print str(err)
		sys.exit()
		
	cascade = cv.Load(haarfile)
	
	if not cascade:
		print "Impossible de charger la cascade de classifieurs à l'emplacement " + haarfile + "!"
		sys.exit(1)
	
	#capture une image avec la webcam
	capture = cv.CreateCameraCapture(webcamID)
	if showCamera == 1:
		cv.NamedWindow("result", 1)
	
	if capture:
	   frame_copy = None
	   while True:
	       frame = cv.QueryFrame(capture)
	       if not frame:
	           cv.WaitKey(0)
	           break
	       #FAIRE UN TEST SANS FRAME_COPY DU TOUT
	       if not frame_copy:
	           frame_copy = cv.CreateImage((frame.width,frame.height),
	                                       cv.IPL_DEPTH_8U, frame.nChannels)
			#s'assure que l'origine de l'image est la bonne
	       if frame.origin == cv.IPL_ORIGIN_TL:
	           cv.Copy(frame, frame_copy)
	       else:
	           cv.Flip(frame, frame_copy, 0)
	       
	       detect_and_send(frame_copy, cascade)
	
	       if cv.WaitKey(1) >= 0:
	           break
	else:
		print "Impossible de capturer une image avec la caméra !"
		cv.WaitKey(0)
	if showCamera == 1:
		cv.DestroyWindow("result")
	moyenne = float(somme / cpt)
	print "Moyenne calcul : " + str(moyenne) + "ms"