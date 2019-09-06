#include<iostream>
#include<vector>
#define ll long long

using namespace std;

ll gcd(ll a, ll b){
	ll L = a>b? a:b;
	ll S = a>b? b:a;
	ll left = L - L/S * S;
	while(left!=0){
	L = S;
	S = left;
	left = L - L/S * S;
	}
	return S;
}

int extended_euclidean(ll a, ll b,ll res[2]){
	ll xy_state[2][2] = {0};
	ll cur_x = 0;
	ll cur_y = 0;
	ll L = a>b? a:b;
	ll S = a>b? b:a;
	ll R = L/S;
	ll left = L - L/S * S;
	xy_state[1][1] += -R;
	xy_state[1][0] += 1;
	xy_state[0][1] = 1;
	while(left!=0){
	L = S;
	S = left;
	left = L - L/S * S;
	R = L/S;
	cur_x= xy_state[0][0]-R*xy_state[1][0];
	cur_y= xy_state[0][1]-R*xy_state[1][1];
	xy_state[0][0] = xy_state[1][0];
	xy_state[0][1] = xy_state[1][1]; 
	xy_state[1][0] = cur_x;
	xy_state[1][1] = cur_y; 
	}
	res[0] = xy_state[0][0];
	res[1] = xy_state[0][1];
	return 0;
}

ll GetModReverse(ll a,ll b){
	ll g = gcd(a,b);
	if(g!=1){
	   return -1;
	}
	ll res[2];
	extended_euclidean(a,b,res);
	return res[0];
}

void test_gcd_extend(){
	ll a = 240;
	ll b = 46;
	ll s = gcd(a,b);
	cout<<s<<'\n';
	ll r[2] = {0};
	extended_euclidean(a,b,r);
	cout<<r[0]<<","<<r[1]<<"\n";
	bool flag = r[0]*a + r[1]*b == s;
	cout<<flag<<"\n";
}


int main(int argc,const char** argv){
	test_gcd_extend();
	ll a;
	ll b;
	while(true){
	cin>>a>>b;
	ll res = GetModReverse(a,b);
	cout<<res<<"\n";
	}
	return 0;
}

static int __ = [](){
	/* std::ios::sync_with_stdio(false); */
	/* cin.tie(NULL); */
	return 0;
}();


