using namespace std;

#define NUM_OF_CHAIN_TYPES 5
#define NUM_OF_INPUT_CHAINS 20
#define NUM_OF_FEATURES 6
#define NUM_OF_NODES 45
#define NUM_OF_CLOUDS 5
#define NUM_OF_ALLOCATED_CHAINS 10
//#define NUM_OF_FEATURES 8

#define MAX_PATH_LENGTH 16

// 需要用const修饰, 不然error: array bound is not an integer constant before ']' token
const int NUM_OF_FIREWALL = 5;
const int NUM_OF_ROUTER = 4;
const int NUM_OF_IDP = 4;
const int NUM_OF_NF = 3;

/* 定义（原论文）五种服务链不同实现方式的feature选择 */
 
//int Firewall[3][NUM_OF_FEATURES] = {
//	{0,0,0,0,0,0,0,0},
//	{1,1,0,0,0,0,0,0},
//	{1,0,1,0,0,0,0,0}
//};
//
//int StrictFirewall[2][NUM_OF_FEATURES] = {
//	{0,0,0,0,0,0,0,0},
//	{1,0,1,0,0,0,0,0}
//};
//
//int SampledDPI[3][NUM_OF_FEATURES] = {
//	{0,0,0,0,0,0,0,0},
//	{1,0,0,1,1,0,0,0},
//	{1,0,0,1,0,1,0,0}
//};
//
//int FullDPI[3][NUM_OF_FEATURES] = {
//	{0,0,0,0,0,0,0,0},
//	{1,0,0,1,1,0,0,0},
//	{1,0,0,1,0,1,0,0}
//};
//
//int StrictFullDPI[3][NUM_OF_FEATURES] = {
//	{0,0,0,0,0,0,0,0},
//	{1,0,0,1,1,0,0,0}
//};

/* 定义（新模型）五种服务链不同实现方式的feature选择 */

int Firewall[3][NUM_OF_FEATURES] = {
	{0,0,0,0,0,0},
	{1,1,0,0,0,0},
	{1,0,1,0,0,0}
};

int StrictFirewall[3][NUM_OF_FEATURES] = {
	{0,0,0,0,0,0},
	{1,0,1,0,0,0},
	{0,0,0,0,0,0}    // 凑整
};

int BasicDPI[3][NUM_OF_FEATURES] = {
	{0,0,0,0,0,0},
	{1,0,0,1,0,1},
	{0,0,0,0,0,0}    // 凑整
};

int FullDPI[3][NUM_OF_FEATURES] = {
	{0,0,0,0,0,0},
	{1,0,0,1,1,0},
	{1,0,0,1,0,1}
};

int StrictFullDPI[3][NUM_OF_FEATURES] = {
	{0,0,0,0,0,0},
	{1,0,0,1,1,0},
	{0,0,0,0,0,0}    // 凑整 
};

int chain_types[NUM_OF_CHAIN_TYPES][3][NUM_OF_FEATURES]={0};


int num_of_ins[NUM_OF_CHAIN_TYPES] = {3, 2, 2, 3, 2};


/* 定义feature与可选节点对应关系 */

enum phy_feature {
	f3 = 2, 
	f5 = 4,
	f6 = 5
}; 

int phy_feature_set[3] = {f3, f5, f6};

int FW_nodes[NUM_OF_FIREWALL] = {37,38,39,40};    // 少了一个 41 以后再说 
int IDP_nodes[NUM_OF_IDP] = {37,38,39,41};

int *service_nodes[NUM_OF_NF] = {FW_nodes, IDP_nodes, IDP_nodes};

struct CFC {
	int src;
	int sink;
	int service_type;
	int ins = 0;
	int phy = -1;    // 第几个物理特征 
	int node = 0;
	int demand = 1;    // 暂时固定
	int path[MAX_PATH_LENGTH] = {0};    // 最长会有 14 个点
	double cost = 0.0;
	
	int update_ins = 0;
	int update_phy = -1;
	int update_node = 0;
	int update_path[MAX_PATH_LENGTH] = {0};
	double update_cost;
} Input_Chains[NUM_OF_INPUT_CHAINS], Allocated_Chains[NUM_OF_ALLOCATED_CHAINS]; 

int realc[NUM_OF_ALLOCATED_CHAINS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

// 交换机直连的边缘云(0 代表没有直连的)(index = 节点号 - 28)
int sw_edge[9] = {-1, -1, 37, -1, -1, 38, -1, -1, 39};

// 交换机直连的 RT(index = 节点号 - 28)
int sw_RT[9] = {44, 44, 44, 45, 45, 45, 42, 42, 42};

// NF 直连的交换机(0 代表没有直连的)(index = 节点号 - 37)
int cloud_sw[NUM_OF_CLOUDS] = {30, 33, 36, -1, -1};

// RT 之间的拓扑(index = 节点号 - 42)(0 代表路径结束) 
int RT1_RT2[5][4] = {
	{43, -1, -1, -1},
	{45, 43, -1, -1},
	{44, 43, -1, -1},
	{45, 44, 43, -1},
	{44, 45, 43, -1}
};

int RT1_RT3[5][4] = {
	{44, -1, -1, -1},
	{43, 44, -1, -1},
	{45, 44, -1, -1},
	{43, 45, 44, -1},
	{45, 43, 44, -1}
};

int RT1_RT4[5][4] = {
	{45, -1, -1, -1},
	{43, 45, -1, -1},
	{44, 45, -1, -1},
	{43, 44, 45, -1},
	{44, 43, 45, -1}
};

int RT2_RT1[5][4] = {
	{42, -1, -1, -1},
	{44, 42, -1, -1},
	{45, 42, -1, -1},
	{44, 45, 42, -1},
	{45, 44, 42, -1}
};

int RT2_RT3[5][4] = {
	{44, -1, -1, -1},
	{42, 44, -1, -1},
	{45, 44, -1, -1},
	{42, 45, 44, -1},
	{45, 42, 44, -1}
};

int RT2_RT4[5][4] = {
	{45, -1, -1, -1},
	{42, 45, -1, -1},
	{44, 45, -1, -1},
	{42, 44, 45, -1},
	{44, 42, 45, -1}
};

int RT3_RT1[5][4] = {
	{42, -1, -1, -1},
	{43, 42, -1, -1},
	{45, 42, -1, -1},
	{43, 45, 42, -1},
	{45, 43, 42, -1}
};

int RT3_RT2[5][4] = {
	{43, -1, -1, -1},
	{42, 43, -1, -1},
	{45, 43, -1, -1},
	{42, 45, 43, -1},
	{45, 42, 43, -1}
};

int RT3_RT4[5][4] = {
	{45, -1, -1, -1},
	{42, 45, -1, -1},
	{43, 45, -1, -1},
	{42, 43, 45, -1},
	{43, 42, 45, -1}
};

int RT4_RT1[5][4] = {
	{42, -1, -1, -1},
	{43, 42, -1, -1},
	{44, 42, -1, -1},
	{43, 44, 42, -1},
	{44, 43, 42, -1}
};

int RT4_RT2[5][4] = {
	{43, -1, -1, -1},
	{42, 43, -1, -1},
	{44, 43, -1, -1},
	{42, 44, 43, -1},
	{44, 42, 43, -1}
};

int RT4_RT3[5][4] = {
	{44, -1, -1, -1},
	{42, 44, -1, -1},
	{43, 44, -1, -1},
	{42, 43, 44, -1},
	{43, 42, 44, -1}
};

int RT_Paths[NUM_OF_ROUTER][NUM_OF_ROUTER][5][4]={0};

int use_cost_of_node[NUM_OF_NODES] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	2, 2, 2, 2, 2, 2, 2, 2, 2, 
	4, 4, 4, 4, 4, 
	8, 8, 8, 8
};

float resource_cost_of_node[NUM_OF_CLOUDS] = {2, 2, 2, 1, 3};

//int chain_failure_cost[NUM_OF_CHAIN_TYPES] = {1, 2, 4, 8, 16};

int feature_failure_cost[NUM_OF_CHAIN_TYPES][NUM_OF_FEATURES] = {
	{32, 8, 32, 0, 0, 0},
	{32, 0, 32, 0, 0, 0},
	{32, 0, 0, 0, 0, 32},
	{64, 0, 0, 0, 64, 16},
	{64, 0, 0, 0, 64, 0}
};

int update_msg_cost = 8;

int num_of_realc = 0;

// 拓扑带宽
int h_s = 20;
int s_r = 40;
int s_e = 40;
int r_r = 60;
int n_r = 60;

int BW[NUM_OF_NODES][NUM_OF_NODES] = {
	/*
	 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45
	*/
	
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	
	/*
	 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45
	*/
	
	{h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r, 0},
	{0, 0, 0, h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r, 0},
	{0, 0, 0, 0, 0, 0, h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_e, 0, 0, 0, 0, 0, 0, s_r, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_e, 0, 0, 0, 0, 0, 0, s_r},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, h_s, h_s, h_s, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_e, 0, 0, s_r, 0, 0, 0},
	
	/*
	 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45
	*/
	
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_e, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_e, 0, 0, 0, 0, 0, 0, n_r, n_r, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_e, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, n_r, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, n_r, 0, 0},
	
	/*
	 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45
	*/
	
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r, s_r, s_r, 0, 0, 0, 0, 0, 0, r_r, r_r, r_r},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, n_r, n_r, r_r, 0, r_r, r_r},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r, s_r, s_r, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, r_r, r_r, 0, r_r},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s_r, s_r, s_r, 0, 0, 0, 0, 0, 0, 0, 0, r_r, r_r, r_r, 0}
};

int RS[NUM_OF_CLOUDS] = {20, 20, 20, 30, 40};

double U = 0.0;
