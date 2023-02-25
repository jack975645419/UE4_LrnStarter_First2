
rem content = content.replace("${{BUILD_NUM}}", sys.argv[1])
rem content = content.replace("${{PIPELINE_ID}}", sys.argv[2])
rem content = content.replace("${{NODES}}", sys.argv[3])
rem content = content.replace("${{DELAY_START}}", sys.argv[4])
rem content = content.replace("${{LOCK}}", sys.argv[5])
rem content = content.replace("${{PREFER_MASK_STR}}", sys.argv[6])
rem content = content.replace("${{TOLERANCE}}", sys.argv[7])
rem 
rem write_file(sys.argv[8], content)

rem						序号	任务名	机器名						延时	锁与解锁	mask	容忍时长	脚本名称	
python hg1_transform.py 11 		testing node1,Engine_Bake_3090_02 	0 		true 		1,1 	1 			hg_1.py
python hg1_transform.py 12 		testing node1,Engine_Bake_3090_02 	100 	true 		1,1 	1 			hg_2.py
python hg1_transform.py 13 		testing node1,Engine_Bake_3090_02 	0 		true 		1,1 	1 			hg_3.py
python hg1_transform.py 14 		testing node1,Engine_Bake_3090_02 	2 		true 		1,1 	1 			hg_4.py
python hg1_transform.py 15 		testing node1,Engine_Bake_3090_02 	0 		true 		1,1 	1 			hg_5.py
			
python hg1_transform.py 11 		testing node1,Engine_Bake_3090_02 	0 		false 		1 		0 			hg_1f.py
python hg1_transform.py 12 		testing node1,Engine_Bake_3090_02 	100 	false 		1 		1 			hg_2f.py
python hg1_transform.py 13 		testing node1,Engine_Bake_3090_02 	0 		false 		1 		1 			hg_3f.py
python hg1_transform.py 14 		testing node1,Engine_Bake_3090_02 	200 	false 		1 		1 			hg_4f.py
python hg1_transform.py 15 		testing node1,Engine_Bake_3090_02 	0 		false 		1 		1 			hg_5f.py
										
python hg1_transform.py -1 		testing node1,Engine_Bake_3090_02 	0 		false 		1 		1 			hg_-1f.py
