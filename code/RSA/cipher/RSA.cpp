#include "RSA.h"

#define DEBUG
RSA::RSA() {
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(phi_n);
	mpz_init(e);
	mpz_init(d);

	// randomly generate two 
	// large prime numbers p,q
	gen_prime(p);
	gen_prime(q);
	// prim test
	mpir_ui one = 1;
	mpz_t _one;
	mpz_init(_one);
	mpz_set_ui(_one, one);
	mpz_t r;
	mpz_init(r);
	mpz_gcd(r, p, q);
	while (mpz_cmp(r, _one) != 0) {
		gen_prime(p);
		gen_prime(q);
		mpz_gcd(r, p, q);
	}

	// n = pq
	mpz_mul(n,p,q);

	// eular function phi(n)= (p-1)(q-1)
	mpz_t tmp_p;
	mpz_t tmp_q;
	mpz_init(tmp_p);
	mpz_init(tmp_q);
	mpz_sub(tmp_p, p, _one);
	mpz_sub(tmp_q, q, _one);
	mpz_mul(phi_n,tmp_p,tmp_q);
	mpz_clear(tmp_p);
	mpz_clear(tmp_q);

	// randomly select one number e from
	// range (1,phi(n)) which satisfies (e,phi(n))=1
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, time(NULL));
	mpz_urandomm(e,state,phi_n);
	mpz_gcd(r, e, phi_n);
	while (mpz_cmp(r,_one)!=0) {
	mpz_urandomm(e,state,phi_n);
	mpz_gcd(r, e, phi_n);
	}

	// use extended eculidean algo to
	// get the key number d which satisfies ed = 1 mod(phi(n))
	mpz_gcdext(_one, NULL, d, phi_n, e);
#ifdef DEBUG
	this->print();
#endif
	mpz_clear(_one);
}

void RSA::print() {
	gmp_printf(" p:%Zd\n q:%Zd\n n:%Zd\n e:%Zd\n d:%Zd\n",
		this->p,
		this->q,
		this->n,
		this->e,
		this->d
		);
}

void RSA::gen_prime(mpz_t prime) {
	gmp_randstate_t state;
	// Mersenne Twister algorithm
	gmp_randinit_mt(state);
	/*
	 The method for choosing a seed is critical if the generated
	 numbers are to be used for important applications, such
	 as generating cryptographic keys.
	*/
	gmp_randseed_ui(state, time(NULL));
	mpz_t tmp;
	mpz_init(tmp);
	mpz_urandomb(tmp, state, 1024); // 1024 bits
	mpz_nextprime(prime, tmp); // it does not guarantee primality
	mpz_clear(tmp); // free space
}

int RSA::encrypt_str(string& input, string& output) {
	mpz_class msg(input, 16);
	// C = m^e (mod n)
	mpz_class en_msg;
	mpz_powm(en_msg.get_mpz_t(),msg.get_mpz_t(),e,n);
	output = en_msg.get_str(16);
	return 0;
}

int RSA::decrypt_str(string& input,string& output) {
	//C^d = (m^e)^d = m (mod n)
	mpz_class en_msg(input, 16);
	mpz_class msg;
	mpz_powm(msg.get_mpz_t(),en_msg.get_mpz_t(),d,n);
	output = msg.get_str(16);
	return 0;
}

int RSA::md5_hash(string& input, string& hash) {
	ifstream is(input,ios_base::in|ios_base::binary|ios_base::ate);
	if (!is.is_open()) {
		throw std::invalid_argument("invalid filename");
		return EXIT_FAILURE;
	}
	u64 buf;
	int buf_size = sizeof u64;
	int size = is.tellg();
	is.seekg(0,ios::beg);
	int block_nums = size / (buf_size);
	if (block_nums * buf_size < size) {
		block_nums += 1;
	}
	int offset = block_nums * buf_size - size;
	memset(&buf, '\0', buf_size);
	// sign file with md5
	md5_byte_t digest[16];
	md5_state_t state;
	md5_init(&state);
	int i = 0;
	while (i < block_nums-1) {
		memset(&buf, '\0', buf_size);
		is.read((char*)& buf, buf_size);
		md5_append(&state, (const md5_byte_t*)& buf, buf_size);
		i++;
		is.seekg(buf_size * i);
	}
	memset(&buf, '\0', buf_size);
	is.read((char*)& buf, buf_size-offset);
	md5_append(&state, (const md5_byte_t*)& buf, buf_size);
	md5_finish(&state, digest);
	char t[32];
	memset(t, '\0', 32);
	for (i = 0; i < 16; i++) {
		sprintf(t + i * 2, "%02x", digest[i]);
	}
	hash = string(t);
#ifdef DEBUG
	cout << endl;
	cout << "digest:"<< hash << endl;
#endif
}

int RSA::sign_file(string& input, string& output) {
	// hash
	string _sign;
	md5_hash(input, _sign);
	// sign with private key
	mpz_class m(_sign, 16);
	mpz_class signature;
	mpz_powm(signature.get_mpz_t(), m.get_mpz_t(), d, n);
	string sign = signature.get_str(16);
	ofstream ofs(output, ios_base::binary);
	if (!ofs.is_open()) {
		throw std::invalid_argument("invalid filename");
		return EXIT_FAILURE;
	}
	ofs.write(sign.c_str(), sign.size());
	return 0;
}

int RSA::verify_file(string& msg_file,string& sign_file) {
	// load signature file
	ifstream is(sign_file, ios_base::binary | ios_base::ate | ios_base::in);
	int size = is.tellg();
	is.seekg(0,ios_base::beg);
	string _sign(size,'\0');
	is.read(&_sign[0],size);
	// get md5 hash code of msg_file
	string hash;
	md5_hash(msg_file, hash);
	// verify
	mpz_class origin(hash, 16);
	mpz_class _origin;
	mpz_class sign(_sign, 16);
	mpz_powm(_origin.get_mpz_t(), sign.get_mpz_t(), e, n);
	string res = _origin.get_str(16);
#ifdef DEBUG
	cout << "verified hash:" <<res << endl;
	cout << "original hash:" <<hash<< endl;
#endif
	if (mpz_cmp(origin.get_mpz_t(), _origin.get_mpz_t()) != 0) {
		return -1;
	}
	return 0;
}
