import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
from scipy.signal import spectrogram
from scipy.stats import pearsonr

import warnings
warnings.filterwarnings("ignore")

def draw_spectogram(filename):
	s, fs = sf.read(filename)
	s.min(), s.max()
	t = np.arange(s.size) / fs
	f, t, sgr = spectrogram(s, fs, nperseg=400, noverlap=240, nfft=512)
	sgr_log = 10 * np.log10(sgr+1e-20) 
	plt.figure(figsize=(9,3))
	plt.pcolormesh(t,f,sgr_log)
	plt.title(filename[13:]+' spectogram', weight='bold')
	plt.gca().set_xlabel('Čas [s]')
	plt.gca().set_ylabel('Frekvence [Hz]')
	cbar = plt.colorbar()
	cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)
	plt.tight_layout()
	plt.savefig('../pdf/spektogram.jpg')

def get_features(filename):
	s, fs = sf.read(filename)
	s.min(), s.max()
	t = np.arange(s.size) / fs
	f, t, sgr = spectrogram(s, fs, nperseg=400, noverlap=240, nfft=512)
	sgr_log = 10 * np.log10(sgr+1e-20)
	nsegments = (sgr.shape)[1]
	features = np.zeros((16,nsegments))
	for x in range(16):
		for y in range(nsegments):
			sucet = 0
			for i in range(16):
				sucet += sgr_log[i+x*16][y]
			features[x][y] = sucet
	return features

def get_pearson_score(q_f, s_f):
	score = []
	q_f = np.transpose(q_f)						# transponujem maticu parametrov query a aj sentence
	s_f = np.transpose(s_f)						# aby obe boli rozmeru Nx16 a aby som ich mohol jednoducho
												# vložiť do pearsonr()
	score_len = s_f.shape[0] - q_f.shape[0]		# max pocet cyklov bude pocet segmentov vety - pocet segmentov
												# slova, pretoze na konci vety by nebolo dost zvysnych segmentov 
												# na porovnanie so segmentmi slova
	sum = 0
	n_sum = 0
	for pp in range(score_len):			# prechod vsetkych potencionalych pozicii
		for q in range(q_f.shape[0]):	# prechod cez vsetky segmenty queue
			r, p_value = pearsonr(q_f[q],s_f[pp + q]) # pearsonova funkcia
			if not np.isnan(r):		# ak nie je ziadna variacia v parametroch vracia nan
				sum += r 			# priratanie vysledku porovnania aktualneho segmentu
				n_sum += 1
		sum = sum / n_sum			# predelenie poctom priratanych vysledokv
		score.append(sum)			# vlozenie do pola, toto je koniec vypoctu score pre jednu pp
		sum = 0
		n_sum = 0
	return score

def plot_graphs(s_file,q1_file,q2_file):
	s, fs = sf.read(s_file)
	s.min(), s.max()
	t = np.arange(s.size) / fs
	t = t*100
	plt.figure()
	plt.suptitle('"shipbuilding" and "activities" vs '+s_file, weight='bold')
	ax1 = plt.subplot(311)
	plt.plot(t,s)
	plt.locator_params(axis='x', nbins=int(max(t)//100+1))
	plt.locator_params(axis='y', nbins=5)
	ax1.set_xlabel('$t[s]$')
	ax1.set_ylabel('$signal$')
	ax1.set_xticklabels(np.arange(int(max(t)//100+1)))
	plt.xlim(left=0, right=max(t))
	s_f = get_features(s_file)
	ax2 = plt.subplot(312, sharex=ax1)
	plt.pcolormesh(s_f)
	ax2.set_xlabel('$t[s]$')
	ax2.set_ylabel('$features$')
	plt.gca().invert_yaxis()
	q1_f = get_features(q1_file)
	q2_f = get_features(q2_file)
	score1 = get_pearson_score(q1_f, s_f)
	score2 = get_pearson_score(q2_f, s_f)
	ax3 = plt.subplot(313, sharex=ax1)
	plt.plot(score1)
	plt.plot(score2)
	plt.locator_params(axis='y', nbins=8)
	ax3.set_xlabel('$t[s]$')
	ax3.set_ylabel('$scores$')
	plt.legend(['shipbuilding', 'activities'])
	plt.xlim(left=0)
	file = '../pdf/'+s_file[13:]+'.jpg'
	plt.savefig(file)

def find_hits(scores, sentence, query ,prah):
	data_s, fs_s = sf.read(sentence)
	data_q, fs_q = sf.read(query)
	hit = []
	counter = 0
	last_hit = False
	for i in range(len(scores)):
		if scores[i] >= prah:
			if last_hit == False:
				hit_end = 160*i+len(data_q)
				hit = data_s[160*i:hit_end]
				sf.write('../hits/'+query[11:13]+'_'+(sentence[13:len(sentence)-4])+'_'+str(counter)+'.wav',hit,fs_s)
				print('Hit: '+query[11:13]+'_'+(sentence[13:len(sentence)-4])+'_'+str(counter)+' At samples: '+str(i*160)+'-'+str(hit_end))
				counter += 1
				last_hit = True
			else:
				if 160*i >= hit_end:
					last_hit = False

def get_hits(s):
	scores1 = get_pearson_score(get_features(queries[0]),get_features(s))
	scores2 = get_pearson_score(get_features(queries[1]),get_features(s))
	find_hits(scores1, s, queries[0], 0.78)
	find_hits(scores2, s, queries[1], 0.76)

queries = ['../queries/q1.wav','../queries/q2.wav']
sentences = ['../sentences/sa1.wav','../sentences/sa2.wav','../sentences/si938.wav',
			'../sentences/si2198.wav','../sentences/sx38.wav','../sentences/sx128.wav',
			'../sentences/sx308.wav','../sentences/sx398.wav','../sentences/si1568.wav',
			'../sentences/sx218.wav']
for s in sentences:
	get_hits(s)
"""
for i in range(10):
	plot_graphs(sentences[i],queries[0],queries[1])
"""
#draw_spectogram('../sentences/sa1.wav')