#include <fstream>
#include <string>
#include "cost.cpp"
#include "update.cpp"
#include <set>

bool cmp(bool f_choice[], bool model[]) {
//	cout<<model[0]<<" "<<model[1]<<" "<<model[2]<<" "<<model[3]<<" "<<model[4]<<" "<<model[5]<<endl;
	for(int i = 0; i < NUM_OF_FEATURES; ++i) {
		if(f_choice[i] != model[i]) return false;
	}
	return true;
}

int composite(int path[], bool p_choice[][5][NUM_OF_PATH], int src, int sink) {
	int p = 0, q = 1, k = 0, node = -1;
	bool source = true;
	path[k] = src;
	while(source || path[k] != sink) {
//		cout<<p<<" "<<q<<" "<<endl;
		source = false;
		bool has_one = false;    // 这两个物理资产之间有 link 
		set<pair<int,int> > link;
		for(int l = 0; l < NUM_OF_PATH; ++l) {
			if(p_choice[p][q][l] == true) {
				has_one = true;
				link.insert(make_pair(from[l], to[l]));
			}
		}
		
		set<pair<int,int> >::iterator it;
//		for(it = link.begin(); it != link.end(); it++) {
//			cout<<it->first<<" "<<it->second<<endl;
//		}
//		cout << endl;
		while(!link.empty()) {
//			cout << "非空" << endl;
			for(it = link.begin(); it->first != path[k]; it++) {} 
			++k;
			path[k] = it->second;
//			cout << path[k] << " ";
			if(path[k] >= 41) {
				node = path[k];
//				node_used[node - 41] += 1;
			}
			link.erase(it);
		}	
		
		if(q == 4) break;
		if(has_one) {
			p = q;
			q = 0;
		}
		else {
			q++;
		}
	}
//	for(k = 0; k < MAX_PATH_LENGTH; ++k) {
//		cout << path[k] << " ";
//	} 
//	cout<<endl;
	return node;
}

void allocated_chains() {
	string s;
	int pair_count = 0, i = 0, c = 0;
	ifstream infile; 
	
	
	// 读取原先服务链的 src sink type demand
//	cout<<"读取原先服务链的 src sink type demand 开始"<<endl;
	infile.open(dir + "former_cfc.txt");    // 需要用双反斜杠, 或一个正斜杠 
//	cout<<"读取成功"<<endl;
	while(getline(infile, s)) {  
    	bool temp = false;
    	int pos = 0, data = 0;
    	int d[4] = {0};
        for(int i = 0; i < s.length(); ++i) {  
	        while(s[i] >= '0' && s[i] <= '9') {      // 当前字符是数字   
	            temp = true;      // 读数据标志位置位  
	            data *= 10;  
	            data += s[i] - '0';       // 字符在系统以ASCII码存储  
	            ++i;  
	        }  
	        if(temp) {     // 刚读取了数据  
	            d[pos] = data;      // 赋值   
	            data = 0;  
				++pos;  
	            temp = false;     // 标志位复位  
	        }  
	    } 
	    Allocated_Chains[c].src = d[0];
	    Allocated_Chains[c].sink = d[1];
	    Allocated_Chains[c].service_type = d[2] - 1;    // CPLEX 中的类型计数从 1 开始
	    Allocated_Chains[c].demand = d[3] * multiplier;    // 在一开始就要乘以系数, 后面就开始扣除资源了 
	    c++;
	}
//	for(c = 0; c < NUM_OF_ALLOCATED_CHAINS; ++c) {
//		cout<<Allocated_Chains[c].src<<" "<<Allocated_Chains[c].sink<<" "<<Allocated_Chains[c].service_type<<" "<<Allocated_Chains[c].demand<<endl;
//	} 
	infile.close();
//	cout<<"读取原先服务链的 src sink type demand 成功"<<endl; 

	// 读取 feature 选择 
	//	cout<<"读取 feature 选择开始"<<endl; 
	infile.open(dir + "f_choice.txt");
//	cout<<"读取成功"<<endl;
	c = 0;
	
	while(getline(infile, s)) {
		int j = 0, f = 0;
		bool f_choice[NUM_OF_FEATURES] = {0};
		while(s[j] != '\0') {
			if(s[j] == '1') {
				f_choice[f++] = true;
			}
			else if(s[j] == '0') {
				f_choice[f++] = false;
			}
			j++;
		} 
//		cout<<c<<endl;
//		cout<<f_choice[0]<<" "<<f_choice[1]<<" "<<f_choice[2]<<" "<<f_choice[3]<<" "<<f_choice[4]<<" "<<f_choice[5]<<endl;

		// 推断出 ins
		int num = 0;
		for(; num < num_of_ins[Allocated_Chains[c].service_type]; ++num) {
			if(cmp(f_choice, chain_types[Allocated_Chains[c].service_type][num])) {
				Allocated_Chains[c].ins = num;
//				cout<<"ins："<<num<<endl;
				break;
			}
		}
		// 推断出 phy
		int phy = 0;
		for(int f : phy_feature_set) {
			if(f_choice[f] == true) {
				Allocated_Chains[c].update[num].uphy = phy;
//				cout << Allocated_Chains[c].service_type << " " << endl;
//				cout<<"phy："<<phy<<endl;
//				cout << Allocated_Chains[c].ins << " = " << num << endl;
//				cout << Allocated_Chains[c].update[Allocated_Chains[c].ins].uphy << " = " << Allocated_Chains[c].update[num].uphy << endl;
			}
			phy++;
		}
		c++;
	}
	 
	infile.close(); 
//	cout<<"读取 feature 选择成功"<<endl; 

	
	infile.open(dir + "raw.txt"); 
	
	// 读取路径段 
//	cout<<"读取路径段开始"<<endl; 
	getline(infile, s);
	
	while(s[i] != '\0') {
		if(s[i] == '<') {
			++i;
			while(s[i] != ' ') {
				from[pair_count] *= 10;
				from[pair_count] += s[i] - '0';
				++i;
			}

			++i;
			while(s[i] != '>') {
				to[pair_count] *= 10;
				to[pair_count] += s[i] - '0';
				++i;
			}
			++pair_count;
		}
		++i;
	}
//	cout<<"读取路径段成功"<<endl; 
	
//	for(pair_count = 0; pair_count < NUM_OF_PATH; ++pair_count) {
//		cout<<from[pair_count]<<" "<<to[pair_count]<<endl;
//	}

	// 存储路径选择
//	cout<<"存储路径选择开始"<<endl; 
	c = 0;
	i = 0; 
	bool p_choice[5][5][NUM_OF_PATH] = {0};
	
	while(c < NUM_OF_ALLOCATED_CHAINS) {
		bool first = true;
		for(int p = 0; p < 5; ++p) {
			for(int q = 0; q < 5; ++q) {
				getline(infile, s);

				pair_count = 0;
				int j = 0;
				while(s[j] != '\0') {
					if(s[j] == '1') {
						p_choice[p][q][pair_count++] = true;
					}
					else if(s[j] == '0') {
						p_choice[p][q][pair_count++] = false;
					}
					j++;
				}
			}	
		}
//		for(int p = 0; p < 5; ++p) {
//			for(int q = 0; q < 5; ++q) {
//				for(int l = 0; l < NUM_OF_PATH; ++l) {
//					cout<<p_choice[p][q][l]<<" ";
//				}
//				cout<<endl;
//			}	
//		}
		
		// 合成路径 
//		cout<<"合成路径"<<c<<"开始"<<endl;
		Allocated_Chains[c].update[Allocated_Chains[c].ins].unode = composite(Allocated_Chains[c].update[Allocated_Chains[c].ins].upath, p_choice, Allocated_Chains[c].src, Allocated_Chains[c].sink);
//		cout << "---" << endl;
//		cout << Allocated_Chains[c].update[Allocated_Chains[c].ins].uphy << endl;
		updateCapacity(Allocated_Chains, c, Allocated_Chains[c].ins);    // 必须在下一句之前 
		Allocated_Chains[c].node = Allocated_Chains[c].update[Allocated_Chains[c].ins].unode;
		
//		cout<<"合成路径"<<c<<"成功"<<endl;
		
//		cout << "------" << endl;
		// 更新资源和链链路 
//	    cout<<"A 初始 - "<<c<<endl; 
		updateTraffic(Allocated_Chains[c].path, Allocated_Chains[c].update[Allocated_Chains[c].ins].upath, Allocated_Chains[c].demand, -1, Allocated_Chains[c].update[Allocated_Chains[c].ins].uphy);
	    Allocated_Chains[c].phy = Allocated_Chains[c].update[Allocated_Chains[c].ins].uphy; 
		memcpy(Allocated_Chains[c].path, Allocated_Chains[c].update[Allocated_Chains[c].ins].upath, 4 * MAX_PATH_LENGTH);
	    memcpy(Allocated_Chains[c].ini_path, Allocated_Chains[c].path, 4 * MAX_PATH_LENGTH);
//	    cout << "---------" << endl;
	    
		Allocated_Chains[c].fT = Allocated_Chains[c].update[Allocated_Chains[c].ins].uT = singleCost(c, Allocated_Chains, Allocated_Chains[c].ins);    // 把这里换一个评估函数 
//		cout << "------------" << endl;
		c++;
	}
//	cout<<"存储路径选择成功"<<endl;
	infile.close();

} 

void input_chains() {    // 输入服务链参数(源、目、类型) 

	string s;
	ifstream infile; 
    infile.open(dir + "input_chains.txt"); 
	int c = 0; 
    while(getline(infile, s)) {  
    	bool temp = false;
    	int pos = 0, data = 0;
    	int d[4] = {0};
        for(int i = 0; i < s.length(); ++i) {  
	        while(s[i] >= '0' && s[i] <= '9') { 
	            temp = true;   // 读数据标志位置位  
	            data *= 10;  
	            data += s[i] - '0';
	            ++i;  
	        }  
	          
	        if(temp) {     // 刚读取了数据  
	            d[pos++] = data;      // 赋值   
	            data = 0;   
	            temp = false;     // 标志位复位  
	        }  
	    } 
	    Input_Chains[c].src = d[0];
	    Input_Chains[c].sink = d[1];
	    Input_Chains[c].service_type = d[2] - 1;
	    Input_Chains[c].demand = d[3] * multiplier;
//	    cout<<Input_Chains[c].src<<" "<<Input_Chains[c].sink<<" "<<Input_Chains[c].service_type<<" "<<Input_Chains[c].demand<<endl;
	    c++;
    }  
	infile.close();
}

void read() {
	
	// 服务链类型对应的实现方法 
	memcpy(chain_types[0], Firewall, sizeof(Firewall));
	memcpy(chain_types[1], StrictFirewall, sizeof(StrictFirewall));
	memcpy(chain_types[2], BasicDPI, sizeof(BasicDPI));
	memcpy(chain_types[3], FullDPI, sizeof(FullDPI));
	memcpy(chain_types[4], StrictFullDPI, sizeof(StrictFullDPI));
	
	// RT 之间的路径集合(index = 节点号 - 37)
//	memset(RT_Paths[0][0], 0, sizeof(RT1_RT2));
	memcpy(RT_Paths[0][1], RT1_RT2, sizeof(RT1_RT2));
	memcpy(RT_Paths[0][2], RT1_RT3, sizeof(RT1_RT3));
	memcpy(RT_Paths[0][3], RT1_RT4, sizeof(RT1_RT4));
	
	memcpy(RT_Paths[1][0], RT2_RT1, sizeof(RT2_RT1));
//	memset(RT_Paths[1][1], 0, sizeof(RT1_RT2));
	memcpy(RT_Paths[1][2], RT2_RT3, sizeof(RT2_RT3));
	memcpy(RT_Paths[1][3], RT2_RT4, sizeof(RT2_RT4));
	
	memcpy(RT_Paths[2][0], RT3_RT1, sizeof(RT3_RT1));
	memcpy(RT_Paths[2][1], RT3_RT2, sizeof(RT3_RT2));
//	memset(RT_Paths[2][2], 0, sizeof(RT1_RT2));
	memcpy(RT_Paths[2][3], RT3_RT4, sizeof(RT3_RT4));
	
	memcpy(RT_Paths[3][0], RT4_RT1, sizeof(RT4_RT1));
	memcpy(RT_Paths[3][1], RT4_RT2, sizeof(RT4_RT2));
	memcpy(RT_Paths[3][2], RT4_RT3, sizeof(RT4_RT3));
//	memset(RT_Paths[3][3], 0, sizeof(RT1_RT2));

	memset(node_vnf_demand, 0.0, sizeof(node_vnf_demand));
	
	allocated_chains();
//	allocated_paths();
	
	input_chains();
}
