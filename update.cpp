bool checkTraffic(int path[], int upath[], float demand, int phy, int uphy) {
	bool flag = true;
	float dmd = demand; 
	for(int step = 1; path[step] > 0; ++step) {
		if(path[step-1] >= 41) {
			dmd *= prop[phy];
		}
		BW[path[step]-1][path[step-1]-1] += dmd;
		BW[path[step-1]-1][path[step]-1] += dmd;
	}
	dmd = demand;
	for(int step = 1; upath[step] > 0; ++step) {
		if(upath[step-1] >= 41) {
//			cout<<"压缩前："<<dmd<<"压缩比例："<<prop[uphy]<<endl;
			dmd *= prop[uphy];
//			cout<<"压缩后："<<dmd<<endl; 
		}
		if(BW[upath[step]-1][upath[step-1]-1] < dmd) {
//			cout<<upath[step-1]<<" "<<upath[step]<<" "<< BW[upath[step]-1][upath[step-1]-1]<<" 小于 "<<dmd<<endl;
			flag = false;
		}
//		else {
//			cout<<upath[step-1]<<" "<<upath[step]<<" "<< BW[upath[step]-1][upath[step-1]-1]<<" 可以提供 "<<dmd<<endl;			
//		}
	}
	dmd = demand;
	for(int step = 1; path[step] > 0; ++step) {
		if(path[step-1] >= 41) {
			dmd *= prop[phy];
		}
		BW[path[step]-1][path[step-1]-1] -= dmd;
		BW[path[step-1]-1][path[step]-1] -= dmd;
	}
	return flag;
}

void updateTraffic(int path[], int upath[], float demand, int phy, int uphy) {
	float dmd = demand; 
	for(int step = 1; path[step] > 0; ++step) {
//		cout<<path[step-1]<<" "<<path[step]<<" "<< BW[path[step]-1][path[step-1]-1]<<" 扣除 "<<dmd<<endl;
		if(path[step-1] >= 41) {
			dmd *= prop[phy];
		}
		BW[path[step]-1][path[step-1]-1] += dmd;
		BW[path[step-1]-1][path[step]-1] += dmd;
	}
	dmd = demand; 
	for(int step = 1; upath[step] > 0; ++step) {
//		cout<<upath[step-1]<<" "<<upath[step]<<" "<< BW[upath[step]-1][upath[step-1]-1]<<" 扣除 "<<dmd<<endl;
		if(upath[step-1] >= 41) {
			dmd *= prop[uphy];
		}
		BW[upath[step]-1][upath[step-1]-1] -= dmd;
		BW[upath[step-1]-1][upath[step]-1] -= dmd;
	}
}

bool checkCapacity(struct CFC Chains[], int i, int ins) {
	bool flag = false;
	int node = Chains[i].node, unode = Chains[i].update[ins].unode;
	int phy = Chains[i].phy, uphy = Chains[i].update[ins].uphy;
	float demand = Chains[i].demand;
	
	if(node > 0) {
		if(node != 41) {
			node_vnf_demand[node - 42][phy] -= demand;
		}
		else CAP += demand;
	}
	
	if(unode != 41) {
		if((RS[unode - 42][0] >= node_resource[unode - 42][0] && RS[unode - 42][1] >= node_resource[unode - 42][1]) ||
(int)((node_vnf_demand[unode - 42][uphy] + demand + unit_rps[uphy] - 1)/unit_rps[uphy]) == node_vnf_count[unode - 42][uphy]) {
		  	flag = true;
		}	
	}
	else {
		if(demand <= CAP) flag = true;
	}
	
	if(node > 0) {
		if(node != 41) {
			node_vnf_demand[node - 42][phy] += demand;
		}
	}
	else CAP -= demand;
	
	return flag;
}

void updateCapacity(struct CFC Chains[], int i, int ins) {
	int tmp;
	int node = Chains[i].node, unode = Chains[i].update[ins].unode;
	int phy = Chains[i].phy, uphy = Chains[i].update[ins].uphy;
	float demand = Chains[i].demand;
	
	if(node > 0) {
		if(node != 41) {
			node_used[node - 41] -= 1;
			node_vnf_demand[node - 42][phy] -= demand;
			tmp = node_vnf_count[node - 42][phy];
			node_vnf_count[node - 42][phy] = (int)((node_vnf_demand[node - 42][phy] + unit_rps[phy] - 1)/unit_rps[phy]);
			tmp -= node_vnf_count[node - 42][phy];
			if(tmp != 0) {
				for(int j = 0; j < 2; ++j) {
					RS[node - 42][j] += node_resource[phy][j] * tmp;
				}
			}
		}
		else CAP += demand;
	}
	
	if(unode > 0) {    // 更改后的服务链有用到功能节点 
		if(unode != 41) {    // 功能节点是云节点 
			node_used[unode - 41] += 1;
//			cout << "该节点上该 NF 的 demand 变化：" << node_vnf_demand[unode - 42][uphy] << " ";
			node_vnf_demand[unode - 42][uphy] += demand;
//			cout << node_vnf_demand[unode - 42][uphy] << endl;
			tmp = node_vnf_count[unode - 42][uphy];
			node_vnf_count[unode - 42][uphy] = (int)((node_vnf_demand[unode - 42][uphy] + unit_rps[uphy] - 1)/unit_rps[uphy]);

			tmp = node_vnf_count[unode - 42][uphy] - tmp;

			if(tmp != 0) {
				for(int j = 0; j < 2; ++j) {
					RS[unode - 42][j] -= node_resource[uphy][j] * tmp;
				}
			}

		}
		else CAP -= demand;    // 功能节点是物理节点
	}

//	cout << "node_used：" << endl;
//	for(int i = 0; i < NUM_OF_NFNODES; ++i) {
//		cout << node_used[i] << " ";
//	}
//	cout << endl << "node_vnf_count：" << endl;
//	for(int i = 0; i < NUM_OF_CLOUDS; ++i) {
//		for(int j = 0; j < 3; ++j) {
//			cout << node_vnf_count[i][j] << " ";
//		}
//	}
//	cout << endl;
}
