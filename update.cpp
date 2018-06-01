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
			node_vnf_demand[node - 41][phy] -= demand;
		}
		else CAP += demand;
	}
	
	if(unode != 41) {
		if((RS[unode - 41][0] >= node_resource[unode - 41][0] && RS[unode - 41][1] >= node_resource[unode - 41][1]) ||
(int)((node_vnf_demand[unode - 41][uphy] + demand + unit_rps[uphy] - 1)/unit_rps[uphy]) == node_vnf_count[unode - 41][uphy]) {
		  	flag = true;
		}	
	}
	else {
		if(demand <= CAP) flag = true;
	}
	
	if(node > 0) {
		if(node != 41) {
			node_vnf_demand[node - 41][phy] -= demand;
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
			node_vnf_demand[node - 41][phy] -= demand;
			tmp = node_vnf_count[node - 41][phy];
			node_vnf_count[node - 41][phy] = (int)((node_vnf_demand[node - 41][phy] - demand + unit_rps[phy] - 1)/unit_rps[phy]);
			tmp -= node_vnf_count[node - 41][phy];
			if(tmp != 0) {
				for(int j = 0; j < 2; ++j) {
					RS[node - 41][j] += node_resource[phy][j] * tmp;
				}
			}
		}
		else CAP += demand;
	}
	
	if(unode > 0) {
		if(unode != 41) {
			node_vnf_demand[unode - 41][uphy] += demand;
			tmp = node_vnf_count[unode - 41][uphy];
			node_vnf_count[unode - 41][uphy] = (int)((node_vnf_demand[unode - 41][uphy] + demand + unit_rps[uphy] - 1)/unit_rps[uphy]);
			tmp = node_vnf_count[unode - 41][uphy] - tmp;
			if(tmp != 0) {
				for(int j = 0; j < 2; ++j) {
					RS[unode - 41][j] -= node_resource[uphy][j] * tmp;
				}
			}
		}
		else CAP -= demand;
	}
}
