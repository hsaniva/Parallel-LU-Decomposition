all: pthreadImpl openMP sequential
sequential: sequential.o
	g++ -Wall -Werror -o sequential sequential.o
sequential.o: sequential.cpp
	g++ -Wall -Werror -c sequential.cpp	
pthreadImpl : pThread_Impl.o
	g++ -Wall -Werror -o pthreadImpl pThread_Impl.o -lpthread
openMP: openMP_Impl.o
	g++ -Wall -Werror openMP_Impl.o -o openMP -fopenmp
pThread_Impl.o: pThread_Impl.cpp
	g++ -Wall -Werror -c pThread_Impl.cpp
openMP_Impl.o: openMP_Impl.cpp
	g++ -Wall -Werror -c openMP_Impl.cpp -fopenmp
run: all
	@clear
	@./start.sh
clean:
	@rm -f openMP
	@rm -f pthreadImpl
	@rm -f sequential

	@rm -f openMP_Impl.o
	@rm -f pThread_Impl.o
	@rm -f sequential.o


	@rm -f pThread_Impl.o
	@rm -f openMP_Impl.o
	@rm -f pthreadImpl
	@rm -f openMP
	@rm -f sequential.o
	@rm -f sequential
	@rm -rf OutputFiles
	@mkdir OutputFiles
	@echo "====================="
	@echo "| All files cleaned |"
	@echo "====================="
