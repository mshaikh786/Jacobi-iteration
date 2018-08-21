#!/usr/bin/env python3

import os,sys,getopt

def usage():
	print (sys.argv[0] + ' -i <input directory path> -o <output directory path>')

def arg_parse(argv):
	""" This function parses the command line arguments:
		Possible options:
			-h | --help	calls the usage() fuction
			-i | --idir	Absolute path to input directory where raw files and common_header 
					is located for constructing the VTK files
			-o | --odir	Absolute path to output directory where VTK output is to be 
					written
			-p | --parts	Number of paritions the data for each iteration is to be read from.
					This is equal to the number of MPI ranks the simulation was executed.
	"""

	indir='./raw_files'
	outdir='./vtk_output'
	num_parts=1
	try:
		opts,args=getopt.getopt(argv,"hi:o:p:",["idir=","odir=","parts="])
		for opt,arg in opts:
			if opt in ['h','--help']:
				usage()
			elif opt in ['-i','--idir=']:
				indir=arg
			elif opt in ['-o','--odir=']:
				outdir=arg
			elif opt in ['-p','--parts=']:
				num_parts=arg
			else:
				print ('unrecognized option')
				usage()
				sys.exit(2)
		os.makedirs(outdir,mode=0o777,exist_ok=True)

		return indir,outdir,int (num_parts)

	except getopt.GetoptError as err:
		print(str(err))
		usage()
		sys.exit(2)



def read_header (indir):
	"""
		This fuction reads the common header into a object

		[In]: 
			indir		Absolute path to input directory for raw_files
		[out]:
			return val	returns a list object with the contents of the file read in
	"""

	filename=str(indir+'/common_header.vtk')
	fp= open(filename,'r+')
	H=fp.readlines()
	fp.close()
	return H


def write_header (outdir,H,chkpoint_indx):
	"""

	"""
	suffix=str(100*chkpoint_indx)
	filename=str(outdir+'/'+'iter'+suffix+'.vtk')
	fp = open(filename,'w+')
	for line in H:
		fp.write(line)
	fp.close()

def read_parms(indir):
	"""
	"""

	filename=str(indir+'/parms.sim')
	fp = open(filename,'r+')
	D = fp.readlines()
	fp.close()

	P=dict()
	for line in D:
		tmp=line.strip().split('=')
		P[tmp[0].strip()]=int(tmp[1].strip())
	return (P)

def read_celldata(indir,numranks,chkpoint_indx):
	"""

	"""
	data=list()
	suffix=str(100*chkpoint_indx)
	for i in range(numranks):
		D=list()
		filename=str(indir+'/'+str(i)+'/'+'iter'+suffix)
		fp=open(filename,'r+')
		tmp = fp.readlines()
		fp.close()

		for line in tmp:
			D.append(line.strip().split(' ')) 

		for p in range(len(D)):
			for q in range(len(D[p])):
				data.append(float(D[p][q]))
		del tmp,D
	return(data)

def write_celldata(outdir, Temp, chkpoint_indx):
	"""

	"""
	suffix=str(100*chkpoint_indx)
	filename=str(outdir+'/'+'iter'+suffix+'.vtk')
	fp = open(filename,'a+')
	for line in Temp:
		fp.write(str(line))
		fp.write('\n')
	fp.close()

if __name__ == '__main__':
	indir,outdir,num_parts=arg_parse(sys.argv[1:])
	
	P=read_parms(indir)
	header=read_header(indir)
	for i in range(P['num_checkpoint']):
		write_header(outdir,header,i)

	for i in range(P['num_checkpoint']):
		T=read_celldata(indir,P['numranks'],i)
		write_celldata(outdir,T,i)
		del T
