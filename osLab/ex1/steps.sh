0. touch .hello_there         		# 0. make the file

1. chmod -w .hello_there    		  # 1. remove write permission

2. kill -SIGCONT 6313        		  # 2. send a SIGCONT signal to the process with PID 6313

3. export ANSWER="42"        		  # 3. export an environmental variable

4. mkfifo magic_mirror        		# 4. make a named pipe

5. exec 99>&1            		      # 5. file descriptor 99 is the duplicate for 1 (stdout)

6. ./challenge6		 		            # 6. make two pipes and use dup2 
                        					# to make file descriptors 33,34 - 53,54
                        			 		# and execve to execute riddle

7. ln .hello_there .hey_there    	# 7. create hard link .hey_there to file .hello_there

8. touch bf00
   echo -n "hello, operating systems lab" | dd of=bf00 bs=1 seek=1073741824 conv=notrunc     	
   for i in $(seq -w 1 9); do cp bf00 bf0$i; done   
                        					# 8. write file bf00 in offset declared by seek
                        					# bash script to copy bf00 to other 9 files

9. nc -l -p 49842        		      # 9. listen to port 49842 for connections

10-11. 	ps aux | grep riddle    	
 	dd if=/proc/4123/mem bs=4096 skip=33069692894 count=1 | strings   	
                        					# 10-11. ps to get the <PID>
                        					# where 4123 is the <PID> and skip is selected ...
                        					# mmap return value: 0x7b31b77de000
                        					# -> to decimal -> 135453462093824
                        					# -> / 4096 (page table size) -> 33069692894

12. strace -ff ./change 		      # 12. follow children and give input 
                        					# for the /temp/riddle-[...] file and the char 
                        					# in change.c fork and mmap 
                        					# to the shared memory space and write the char

13. chmod +w .hello_there       	# 13. add write permission
    truncate -s 32K .hello_there	# truncate to size 32KB to prevent bus error

14. strace -ff ./fork			        # 14. fork until process gets the desired pid
                        					# then execve the riddle 

15. touch tier2.so			          # 15. create Dynamically Linked 
    gcc -c sample.c			          # shared object Library
    gcc -shared -o tier2.so sample.o	# with function name setup_tier2
