void printChoice() {
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		cout<<"第"<<i+1<<"条服务链："<<Input_Chains[i].service_type<<" "<<Input_Chains[i].ins<<endl;
		cout<<"NF："<<Input_Chains[i].node<<endl;
		cout<<"COST: "<<Input_Chains[i].cost<<endl;
		for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
			cout<<Input_Chains[i].path[step]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	for(int c = 0; c < NUM_OF_ALLOCATED_CHAINS; ++c) {
		if(c < 0) {
			break;
		}
		cout<<"第"<<c+1<<"条服务链："<<Allocated_Chains[c].service_type<<" "<<Allocated_Chains[c].ins<<endl;
		cout<<"NF："<<Allocated_Chains[c].node<<endl;
		cout<<"COST: "<<Allocated_Chains[c].cost<<endl;
		for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
			cout<<Allocated_Chains[c].path[step]<<" ";
		}
		cout<<endl;
	}
}

void printBW() {
	for(int i = 0; i < NUM_OF_NODES; ++i) {
		for(int j = 0; j < NUM_OF_NODES; ++j) {
			cout<<BW[i][j]<<" ";
		}
		cout<<endl;
	}
}

void printRS() {
	for(int i = 0; i < NUM_OF_CLOUDS; ++i) {
		cout<<RS[i]<<" ";
	}
}
