#include <signal.h>  
#include <sys/types.h>   
#include <unistd.h> 

#define Random(x) (rand() % x)

int n;    // n 条服务链
int fw[n1+1], idp[n2+1], rt[n3+1];    // sf 可选集合
set<int> fw_in_use;
set<int> idp_in_use;
set<int> idp_in_use;

vector<vector<vector<int>>> paths;    // path 序号集合

// 描述每个 session 的 struct
struct Session {
	int nums;    // sf 个数
	sfType nodes[nums+2];    // 所有结点个数
	int appliance_index[nums+2];
	int path[nums+1];
	vector<long> chain_timer(nums, MAX_LONG);
	struct Update update[nums];
} session[n];

// n 条链, 每条 SF 个数个 timer
// vector<vector<long> > chain_timer(n, vector<long>{});

// sf 类型
enum sfType {FW, IDP, RT};

// 发送 RESET 信号时携带的更新信息
struct Update {
	int session_index;
	int update_index;
	int update_sf;
	int update_prev;
	int update_next;
	double utility;
};

void myhandler(int signo, siginfo_t *si, void *ucontext);
void setTimer(...); 

int main() {

	signal(SIGALRM, multi_timer_manage);    //指定单位定时器过期时执行的函数

	// union sigval val;    // 定义一个携带数据的共用体

	// sigset_t mask;  
 //    sigset_t oldmask;  
 //    sigemptyset(&mask);  
 //    sigaddset(&mask, SIGUSR1);  
 
    //2-block the signal, 不然会被默认方式处理 
    // if (sigprocmask(SIG_BLOCK, &mask, &oldmask) == -1)  
    // {  
    //     handle_error("sigprocmask");  
    // }  

	struct sigaction oldact, act;
	act.sa_sigaction = myhandler;
	// 要想让函数使用第二个函数指针，就需要设置sa_flags为SA_SIGINFO
	act.sa_flags = SA_SIGINFO;    // 表示使用sa_sigaction指示的函数，处理完恢复默认，不阻塞处理过程中到达下在被处理的信号
	// 注册信号处理函数
	sigaction(SIGUSR1, &act, &oldact);

	// 初始化x, z
	init();

	start_calculate();
	set_unit_timer();
	while(1) {
		if(utility < target) {
			printResult();
			break;
		}
	}
	// sigprocmask(SIG_SETMASK, &old, NULL);  //取消阻塞:注意，一定要取消block，否则会捕获不到信号
	return 0;
}

void init() {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j <= session[i].nums; j++) {
			// chain_timer[i].push_back(MAX_INT);
			while(path[j] == -1) {
				session[i].update[j].session_index = i;
				session[i].appliance_index[j+1] = pick_ap(session[i].nodes[j+1]);
				session[i].path[j] = pick_path(session[i].nodes[j], session[i].nodes[j+1]);
			}
		}
	}
	return ;
}

int pick_ap(sfType sf) {
	switch(sf) {
		case FW: return fw[Random(n1)];
		case FW: return idp[Random(n2)];
		case FW: return rt[Random(n3)];
		default: return -1;
	}
}

int pick_path(int i, int j) {
	int total = paths[i][j].size();
	return path[Random(total)];
}

void set_unit_timer() {
	// 单位定时器
	struct itimerval one_timer;

	one_timer.it_interval.tv_sec = 0; //设置单位定时器定时开始时间
	one_timer.it_value.tv_sec = 0; //设置单位定时器初始值

	one_timer.it_interval.tv_usec = 0; //设置单位定时器定时开始时间
	one_timer.it_value.tv_usec = 1; //设置单位定时器初始值

	setitimer(ITIMER_REAL, &one_timer, NULL); //初始化单位定时器
}

void multi_timer_manage()
{
	for(int i = 0; i < n; i++) {
		for(int j = 0; j <= session[i].nums; j++) {
			chain_timer[i][j]--;
		}
	}
}

void start_calculate() {
	pthread_t tidp;
	for(int i = 0; i < n; i++) {
		// 指向线程标识符的指针, 线程属性, 线程运行函数的起始地址, 运行函数的参数(setTimer)
		pthread_create(&tidp, NULL, setTimer, &session[i]);
	}
	return;
}

void myhandler(int signo, siginfo_t *si, void *ucowntext){     // si->si_int可以是服务链编号, si->ptr是存放更改数据的struct
	utility = si->ptr.utilty;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j <= session[i].nums; j++) {
			if(i == si->ptr.session_index && j == si->ptr.update_index) {
				continue;
			}
			chain_timer[i] = 用新的 u 计算;
		}
	}
}

void setTimer(Session &session) {
	for(int j = 0; j < session.nums; j++) {
		if(对应的set个数等于总数)) {
			return ;
		}
		alg3(session, j);
	}
	while(running) {
		for(int j = 0; j < session.nums; j++) {
			if(session.chain_timer[j] <= 0) {
				session.appliance_index[j+1] = session.update[j].update_index;
				session.path[j] = session.update[j].update_prev;
				session.path[j+1] = session.update[j].update_next;
				alg3(session, j);
				union sigval value;  
         		value.sival_ptr = session.update[j];
				sigqueue(getpid(), SIGUSR1, value);    // 向本进程发送一个信号
			}
		}
	}
}

void alg3(Session &session, int j) {

	while(path[j] == -1) {
		int tmp = pick_ap(session.nodes[j+1]);
		if(tmp == session.appliance_index[j+1]) {
			continue;
		}
		session.update[j].update_index = j;
		session.update[j].update_sf = tmp;
		session.update[j].update_prev = pick_path(session.nodes[j], session.nodes[j+1]);
		session.update[j].update_next = pick_path(session.nodes[j+1], session.nodes[j+2]);
		session.update[j].utility = 计算新的 u;
	}
	session.chain_timer[j] = 指数值;
}