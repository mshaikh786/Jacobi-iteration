#!/usr/bin/env python3

import os,sys,getopt,re,subprocess as sb
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


def get_timestamps(indir):
	timestamps=list()
	dirname = os.path.join(indir,'0')
	out = sb.run(['ls','%s' %dirname],stdout=sb.PIPE,stderr=sb.PIPE)
	files = str(out.stdout.decode('utf8')).split()
	if len(files) == 0:
		print(str(out.stderr.decode('utf8')))
		exit()
	else:
		for fname in files:
			timestamps.append(re.findall(r'[0-9]+',fname)[0]);
	return(timestamps)

def write_header (outdir,H,chkpoint_indx):
	"""

	"""
	filename=os.path.join(outdir,'iter'+chkpoint_indx+'.vtk')
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
	for i in range(numranks):
		filename=os.path.join(indir,str(i),'iter'+chkpoint_indx)
		fp=open(filename,'r+')
		tmp = fp.readlines()
		fp.close()
		for l in range(len(tmp)):
			elements = tmp[l].strip().split()
			for e in elements:
				data.append(float(e)) 
	return(data)

def write_celldata(outdir, Temp, chkpoint_indx):
	"""

	"""
	filename=os.path.join(outdir,'iter'+chkpoint_indx+'.vtk')
	fp = open(filename,'a+')
	for t in Temp:
		fp.write("%2.4f " %t)
	fp.write('\n')
	fp.close()


if __name__ == '__main__':
	indir,outdir,num_parts=arg_parse(sys.argv[1:])
	
	P=read_parms(indir)
	header=read_header(indir)
	timestamps=get_timestamps(indir)
	for checkpoint_indx in timestamps:
		write_header(outdir,header,checkpoint_indx)

	for checkpoint_indx in timestamps:
		T=read_celldata(indir,P['numranks'],checkpoint_indx)
		write_celldata(outdir,T,checkpoint_indx)
		del T
