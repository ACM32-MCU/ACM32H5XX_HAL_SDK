#-*-coding:GBK -*- 
# encoding: utf-8

import os
import linecache
import shutil
# 从指定path下递归获取所有文件
#path:根目录路径
#fileList:存放所有路径的集合
#filetype:要空滤的格式 如“.txt”,不过滤传入空“”
def getAllFile(path, fileList, filetype):
    dirList = []    # 存放文件夹路径的列表
    for element in os.listdir(path):
        wholepath = os.path.join(path, element)
        # wholepath = path+'/'+ff
        if os.path.isdir(wholepath):
            dirList.append(wholepath)   # 如果是文件夹,添加到结果文件夹列表中	
        if os.path.isfile(wholepath):
            if element.endswith(filetype):
                fileList.append(wholepath)  # 如果是文件，存到文件列表中
        if os.path.isfile(wholepath):
            if '.uvguix'.endswith(filetype):
                if element.find('.uvguix')>0 :
                    fileList.append(wholepath)  # 如果是文件，存到文件列表中                
                

    for dir in dirList:
        getAllFile(dir, fileList ,filetype)   # 对于dirList列表中的文件夹，递归提取其中的文件，fileList一直在往下传，所有的文件路径都会被保存在这个列表中

def getAllDir(path, dirList, dirtype):
    for root, dirs, files in os.walk(path):
        for dir in dirs:
            if (dir in dirtype):
                dirList.append(os.path.join(root, dir))


#从指定path下递归获取所有文件
#Path_build:编译器的路径
#Path_project:工程路径
#IDE_Type:IDE类型，支持KEIL 和 IDE
def BuildProjet(Path_build,Path_project,IDE_Type): 
	result=False
	Path_output=os.getcwd()+r"\build_log.txt"
	if 'KEIL'==IDE_Type:
		cmd=Path_build+" -j0 -r "+'"'+Path_project+'"'+" -o "+'"'+Path_output+'"'
	else:
		cmd=Path_build+' '+'"'+Path_project+'"'+" -build Debug -log all >"+'"'+Path_output+'"'
	os.system(cmd)
	if CheckBuildIsOK(Path_output,IDE_Type)==True:
		result=True
	else:
		result=False
	return result	



def CheckBuildIsOK(Path_log,IDE_Type):
	f = open(Path_log,"r")   #设置文件对象
	lines=f.readlines()
	have_output=0
	result=False
	if 'KEIL'==IDE_Type:
		key='Error(s)'
		for line in lines:
			if key in line:
				have_output=1
				strlist = line.split('-')	# 用逗号分割str字符串，并保存到列表
				num_eroor=strlist[1].split(key)
				num_eroor=num_eroor[0].replace(" ", "")
				if num_eroor == '0':
					print("KEIL编译通过\n")
					result= True
				else:
					print("KEIL编译错误，Error(s)："+num_eroor+" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n")
					result= False
		if have_output==0:
			print("错误，没有输出信息")
			result= False
	else:
		key='errors'
		for line in lines:
			if key in line:
				have_output=1
				line=line.replace("\n", "")
				strlist = line.split(':')	# 用:分割str字符串，并保存到列表
				num_eroor=strlist[1].replace(" ", "")
				if num_eroor == '0':
					print("IAR编译通过\n")
					result= True
				else:
					print("IAR编译错误，Error(s)："+num_eroor+" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n")
					result= False
		if have_output==0:
			print("错误，没有输出信息")
			result= False
			
	f.close()   #将文件关闭
	os.remove(Path_log)
	return result



def StartBulid(workpath, List1, filetype, Path_build,IDE_Type):
	List1 = []
	getAllFile(path,List1,filetype)
	num_project=len(List1)
	num_current=0
	num_BuildOK=0
	print("编译器类型"+IDE_Type+" 一键编译开始,工程总数量： "+"  "+str(num_project))
	for A in List1:
		num_current+=1
		print("当前工程:("+IDE_Type+") "+str(num_current)+"/"+str(num_project))
		print("开始编译工程："+A)
		
		if BuildProjet(Path_build,A,IDE_Type)==True:
			num_BuildOK+=1

	dirListRemove = []    
	getAllDir(path,dirListRemove,('Objects','Listings')) 
	for removefile in dirListRemove:
		shutil.rmtree(removefile)    
    
	ListRemove = []    # 要删除的文件List  
	getAllFile(path,ListRemove,('.bak','.ddk','.edk','.lst','.lnp','.mpf','.mpj','.obj','.omf','.plg','.rpt','.__i','.crf','.o','.d','.tra','.dep','.iex','.htm','.Aisinochip','.scvd','.uvguix'))
	for removefile in ListRemove:
		os.remove(removefile)
  
	result=IDE_Type+"一键编译结束，成功次数: "+str(num_BuildOK)+"/"+str(num_project)+"  失败次数: "+str(num_project-num_BuildOK)+"/"+str(num_project)		
	return result	
    


def runbuild():
	result_KEIL="";
	result_IAR="";
	List1 = []    # 存放文件夹路径的列表  
	
	result_KEIL=StartBulid(path,List1,".uvprojx",Path_UV4,'KEIL')
	
	result_IAR=StartBulid(path,List1,".ewp",Path_IAR,'IAR')
	
	print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  END!   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.")
	print(result_KEIL)
	print(result_IAR)




Path_UV4=r"C:\Keil_v5\UV4\UV4.exe"
Path_IAR=r"C:\IAR\common\bin\IarBuild.exe"
path=os.getcwd()     
	
runbuild()

os.system('pause')
