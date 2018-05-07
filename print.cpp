void printChoice() {
	for(int i = 0; i < NUM_OF_INPUT_CHAINS; ++i) {
		cout<<"第"<<i+1<<"条服务链："<<Input_Chains[i].service_type<<" "<<Input_Chains[i].ins<<endl;
		cout<<"NF："<<Input_Chains[i].node<<endl;
		for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
			cout<<Input_Chains[i].path[step]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	for(int c: realc) {
		if(c < 0) {
			break;
		}
		cout<<"第"<<c+1<<"条服务链："<<Allocated_Chains[c].service_type<<" "<<Allocated_Chains[c].ins<<endl;
		cout<<"NF："<<Allocated_Chains[c].node<<endl;
		for(int step = 0; step < MAX_PATH_LENGTH; ++step) {
			cout<<Allocated_Chains[c].path[step]<<" ";
		}
		cout<<endl;
	}
}
