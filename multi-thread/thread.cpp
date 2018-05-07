#include <signal.h>  
#include <sys/types.h>   
#include <unistd.h> 

struct itimerval one_timer;

// struct timers
// {
// 	int interval;    //定时时间
// 	void(*handler)();    //处理函数
// };

int chain_timer[n] = {MAX_INT};    // n 条链每条一个计时器

void myhandler(int signo, siginfo_t *si, void *ucontext);
void setTimer(...); 

int main() {
	one_timer.it_interval.tv_sec = 1; //设置单位定时器定时时间
	one_timer.it_value.tv_sec = 1; //设置单位定时器初始值
	setitimer(ITIMER_REAL, &one_timer, NULL); //初始化单位定时器
	signal(SIGALRM, multi_timer_manage); //指定单位定时器定时时间到时执行的函数

	union sigval val;    // 定义一个携带数据的共用体
	struct sigaction oldact, act;
	act.sa_sigaction = myhandler;
	act.sa_flags = SA_SIGINFO;    // 表示使用sa_sigaction指示的函数，处理完恢复默认，不阻塞处理过程中到达下在被处理的信号
	// 注册信号处理函数
	sigaction(SIGUSR1, &act, &oldact);

	char data[100];
	start(...);
	while(increase > 1 && 时间 > 300) {}
	return 0;
}

void multi_timer_manage()
{
	对所有计时器：
		chain_timer[i].interval--;
		如果有到时的：
			把其他timer置MAX_INT;
			

	if(two_timer.interval==0)

		{

		two_timer.interval=2;

		two_timer.handler();

}

void start(...) {
	
	对每条链：
		// 指向线程标识符的指针, 线程属性, 线程运行函数的起始地址, 运行函数的参数(setTimer)
		pthread_create(pthread_t *tidp, const pthread_attr_t *attr, (void*)(*start_rtn)(void*), void *arg);
}

void myhandler(int signo, siginfo_t *si, void *ucowntext){     // si->si_int可以是服务链编号, si->ptr是存放更改数据的struct
	按照*si的值更新选择；
	start(...);
	// printf("%s/n",(char*)(si->si_ptr));

}

void setTimer(...) {
	val.sival_ptr = data; 
	sleep(T);
	取消除自己以外所有线程：
		pthread_cancel(pthread_t tid);    // 取消tid指定的线程 成功返回0
	sigqueue(getpid(), SIGUSR1, val);    // 向本进程发送一个信号
	exit(0);
}