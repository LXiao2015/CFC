#include <fstream>
#include <string>
#include "cost.cpp"
void input() {
	
	// �ѷ������������(Դ��Ŀ�����������͡�ʵ�ַ�ʽ������������NF �ڵ㡢·��)
	string s;
	ifstream infile1; 
	infile1.open("allocated_chains.txt");  
	int c = 0;
    while(getline(infile1, s)) {  
    	bool temp = false;
    	int pos = 0, data = 0;
    	int d[6] = {0};
        for(int i = 0; i < s.length(); ++i) {  
	        while((s[i]>='0')&&(s[i]<='9')) {      //��ǰ�ַ�������   
	            temp = true;      //�����ݱ�־λ��λ  
	            data *= 10;  
	            data += (s[i]-'0');       //�ַ���ϵͳ��ASCII��洢��Ҫ�õ���ʵ��ֵ�����ȥ��0����ASCIIֵ  
	            ++i;  
	        }  
	          
	        if(temp) {     //�ն�ȡ������  
	            d[pos] = data;      //��ֵ   
	            data = 0;  
				++pos;  
	            temp = false;     //��־λ��λ  
	        }  
	    } 
	    Allocated_Chains[c].src = d[0];
	    Allocated_Chains[c].sink = d[1];
	    Allocated_Chains[c].service_type = d[2];
	    Allocated_Chains[c].ins = d[3];
	    Allocated_Chains[c].phy = d[4];
	    Allocated_Chains[c].update_node = Allocated_Chains[c].node = d[5];
	    Allocated_Chains[c].update_cost = Allocated_Chains[c].cost = cost(c, Allocated_Chains);
	    c++;
    }  
	infile1.close();
	
	
	// �������������(Դ��Ŀ������) 
	ifstream infile2; 
    infile2.open("input_chains.txt"); 
	c = 0; 
    while(getline(infile2, s)) {  
    	bool temp = false;
    	int pos = 0, data = 0;
    	int d[3] = {0};
        for(int i = 0; i < s.length(); ++i) {  
	        while((s[i]>='0')&&(s[i]<='9')) {      //��ǰ�ַ�������   
	            temp = true;      //�����ݱ�־λ��λ  
	            data *= 10;  
	            data += (s[i]-'0');       //�ַ���ϵͳ��ASCII��洢��Ҫ�õ���ʵ��ֵ�����ȥ��0����ASCIIֵ  
	            ++i;  
	        }  
	          
	        if(temp) {     //�ն�ȡ������  
	            d[pos++] = data;      //��ֵ   
	            data = 0;   
	            temp = false;     //��־λ��λ  
	        }  
	    } 
	    Input_Chains[c].src = d[0];
	    Input_Chains[c].sink = d[1];
	    Input_Chains[c].service_type = d[2];
//	    cout<<Input_Chains[c].src<<" "<<Input_Chains[c].sink<<" "<<Input_Chains[c].service_type<<endl;
	    c++;
    }  
	infile2.close();

	// ���������Ͷ�Ӧ��ʵ�ַ��� 
	memcpy(chain_types[0], Firewall, sizeof(Firewall));
	memcpy(chain_types[1], StrictFirewall, sizeof(StrictFirewall));
	memcpy(chain_types[2], BasicDPI, sizeof(BasicDPI));
	memcpy(chain_types[3], FullDPI, sizeof(FullDPI));
	memcpy(chain_types[4], StrictFullDPI, sizeof(StrictFullDPI));
	
	// RT ֮���·������(index = �ڵ�� - 42)
	memcpy(RT_Paths[0][1], RT1_RT2, sizeof(RT1_RT2));
	memcpy(RT_Paths[0][2], RT1_RT3, sizeof(RT1_RT3));
	memcpy(RT_Paths[0][3], RT1_RT4, sizeof(RT1_RT4));
	memcpy(RT_Paths[1][0], RT2_RT1, sizeof(RT2_RT1));
	memcpy(RT_Paths[1][2], RT2_RT3, sizeof(RT2_RT3));
	memcpy(RT_Paths[1][3], RT2_RT4, sizeof(RT2_RT4));
	memcpy(RT_Paths[2][0], RT3_RT1, sizeof(RT3_RT1));
	memcpy(RT_Paths[2][1], RT3_RT2, sizeof(RT3_RT2));
	memcpy(RT_Paths[2][3], RT3_RT4, sizeof(RT3_RT4));
	memcpy(RT_Paths[3][0], RT4_RT1, sizeof(RT4_RT1));
	memcpy(RT_Paths[3][1], RT4_RT2, sizeof(RT4_RT2));
	memcpy(RT_Paths[3][2], RT4_RT3, sizeof(RT4_RT3));
	
}
