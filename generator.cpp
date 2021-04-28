#include<iostream>
#include<string>
#include <vector>
#include <sstream>


using namespace std;
struct mosphet 
{
	string name, drain, gate, source, body, type;
};

struct block
{
	vector<mosphet> mos;
	string source,drain;
};

void add_remove_bar(string a, int& count, int& wire);
void PDN(string, string, int&, int&);
void PUN(string,string,int&,int&);
void assign_mos(mosphet & a, string drain, string gate, string source, string name, string type);
string anor(string a);


struct circuit
{
	vector<mosphet> inv;
	vector<mosphet> M;
	string firstS, firstD,lastS,lastD;
};



int main()
{

	string input_function;
	string pun_exp, pdn_exp, pun_exp1, pdn_exp1;
	string invalid;
	string output;
	int count = 0;
	int wire = 0;

	invalid = "The input is invalid";

	cout << "Enter the Boolean Expression: ";
	cin >> input_function;

	vector<string> inputs;


	if (input_function.find(';') != string::npos)
	{
	//the case where multiple inputs exists
	stringstream ss(input_function);
	while (ss.good())
	{
		string substr;
		getline(ss, substr, ';');
		inputs.push_back(substr);
	}
	for (int i = 0; i < inputs.size(); i++)
	{
		add_remove_bar(inputs[i], count, wire);
	}
	}
	else
	{
	add_remove_bar(input_function, count, wire);
	}


	system("pause");

	return 0;


}

void PUN(string inv_input, string input, int& count,int& wire)
{
	vector <circuit> pun;
	mosphet a;
	circuit trans;
	trans.firstS = "vdd";	//first source connected to vdd
	trans.lastD = input[0]; //last drain connected to output
	string s1, s2, s3;

	for (int i = 0; i < inv_input.length(); i++) { //casn be moved to main
		if (inv_input[i] == 39)
		{
			s1 = inv_input[i - 1];
			s1 += "'";
			s2 = inv_input[i - 1];
			s3 = "M" + to_string(++count);
			assign_mos(a, s1, s2, "vdd", s3, "pmos");
			trans.inv.push_back(a);
			s3 = "M" + to_string(++count);
			assign_mos(a, s1, s2, "0", s3, "nmos");
			trans.inv.push_back(a);

		}

	}

	if (inv_input.length() == 1) //for y=x'
	{
		s1 = trans.lastD;
		s2 = inv_input[0];
		s3 = "M" + to_string(++count);
		assign_mos(a, s1, s2, "vdd", s3, "pmos");
		trans.inv.push_back(a);
		s3 = "M" + to_string(++count);
		assign_mos(a, s1, s2, "0", s3, "nmos");
		trans.inv.push_back(a);
	}

	string d, g, s, n;
	bool first_and = true;
	bool flag_all_and = false;
	int count_and = 0;
	mosphet q;
	vector<block> blocks;
	block r;
	if (inv_input.length() > 1) {
		if ((((inv_input[2] == 124) && (inv_input[1] == 39)) || (inv_input[1] == 124)) && isalpha(inv_input[0])) //af first alement is Ored
		{

			if (inv_input[1] == 39) //if after | is invereted
			{
				g = inv_input[0];
				g += "'";
			}
			else
				g = inv_input[0];

			n = "M" + to_string(++count);

			d = "wire" + to_string(++wire);

			s = trans.firstS;

			assign_mos(a, d, g, s, n, "pmos");
			trans.M.push_back(a);
			r.mos.push_back(a);
			blocks.push_back(r);
			r.mos.clear();

		}

	}
	for (int i = 0; i < inv_input.length(); i++) {


		if ((!isalpha(inv_input[i]) && !(inv_input[i] == 38) && !(inv_input[i] == 39)) || (i >= inv_input.length() - 1))
		{
			count_and = 0;
			first_and = true;
			if (!r.mos.empty())
				blocks.push_back(r);
			r.mos.clear();
		}
		if (((inv_input[i] == 124) && (i>=inv_input.length() - 2)) || ((inv_input[i] == 124) && ((inv_input[i + 2] != 38) && (inv_input[i + 3] != 38))))
		{

			n = "M" + to_string(++count);
			if (inv_input[i + 2] == 39) //if after | is invereted
			{
				g = inv_input[i + 1];
				g += "'";
			}
			else
				g = inv_input[i + 1];

			d = "wire" + to_string(++wire);

			assign_mos(a, d, g,"vdd", n, "pmos");
			trans.M.push_back(a);

			r.mos.push_back(a);
			
		}


		if (inv_input[i] == 38) //if &
		{
			count_and += 1;
			if ((count_and <1) || first_and)
			{
				//the one before
				if (inv_input[i - 1] == 39) //if before & is invereted
				{
					g = inv_input[i - 2];
					g += "'";
				}
				else
					g = inv_input[i - 1];

				n = "M" + to_string(++count);
				d = "wire" + to_string(++wire);
				if (first_and) {
					s = trans.firstS;
					first_and = false;
				}
				else
					s = r.mos[r.mos.size() - 1].drain; //the source is the drian of the previous one

				assign_mos(a, d, g, s, n, "pmos");
				trans.M.push_back(a);

				r.mos.push_back(a);

			}

			//the one after &

			if (inv_input[i + 2] == 39) //if after & is invereted
			{
				g = inv_input[i + 1];
				g += "'";
			}
			else
				g = inv_input[i + 1];

			n = "M" + to_string(++count);
			d = "wire" + to_string(++wire);
			if (first_and) {
				s = trans.firstS;
				first_and = false;
			}
			else
				s = r.mos[r.mos.size() - 1].drain; //the source is the drian of the previous one

			assign_mos(a, d, g, s, n, "pmos");
			trans.M.push_back(a);
			r.mos.push_back(a);

		}

	}

	
	for (int i = 0; i < trans.inv.size(); i++)
		cout << trans.inv[i].name << " " << trans.inv[i].drain << " " << trans.inv[i].gate << " " << trans.inv[i].source << " " << trans.inv[i].body << " " << trans.inv[i].type << endl;
	
	cout << endl;
	for (int i = 0; i < blocks.size(); i++)
	{
		
			blocks[i].mos[0].source = "vdd";
			blocks[i].mos[blocks[i].mos.size() - 1].drain = trans.lastD;
		
	}

	for (int i = 0; i < blocks.size(); i++)
		for (int j = 0; j < blocks[i].mos.size(); j++)
			cout << blocks[i].mos[j].name << " " << blocks[i].mos[j].drain << " " << blocks[i].mos[j].gate << " " << blocks[i].mos[j].source << " " << blocks[i].mos[j].body << " " << blocks[i].mos[j].type << endl;

}



void PDN(string input1, string input, int& count, int& wire)
{
	vector <circuit> pun;
	mosphet a;
	circuit trans;
	trans.firstD = input[0];	//first source connected to vdd
	trans.lastS = "0" ; //last drain connected to output
	string s1, s2, s3;
	string inv_input;

	inv_input = anor(input1);


	string d, g, s, n;
	bool first_or = true;
	bool flag_all_and = false;
	int count_or = 0;
	mosphet q;
	vector<block> blocks;
	block r;
	string last_source;
	last_source = "y";

	if (inv_input.length() > 1) {
		if ((((inv_input[2] == 38) && (inv_input[1] == 39)) || (inv_input[1] == 38)) && isalpha(inv_input[0])) //af first alement is anded
		{

			if (inv_input[1] == 39) //if after & is invereted
			{
				g = inv_input[0];
				g += "'";
			}
			else
				g = inv_input[0];

			n = "M" + to_string(++count);

			s = "wire" + to_string(++wire);

			d = trans.firstD;

			assign_mos(a, d, g, s, n, "nmos");
			trans.M.push_back(a);
			r.mos.push_back(a);
			blocks.push_back(r);
			last_source = r.mos[0].source;

			r.mos.clear();

		}

	}
	for (int i = 0; i < inv_input.length(); i++) {



		if ((!isalpha(inv_input[i]) && !(inv_input[i] == 124) && !(inv_input[i] == 39)) || (i == inv_input.length() - 1))
		{
			count_or = 0;
			first_or = false;
			if (!r.mos.empty())
			{
				blocks.push_back(r);
				last_source = r.mos[0].source;
			}
			r.mos.clear();
		}

		if (((inv_input[i] == 38) && (i >= inv_input.length() - 2)) || ((inv_input[i] == 38) && ((inv_input[i + 2] != 124) && (inv_input[i + 3] != 124))))
		{

			n = "M" + to_string(++count);
			if (inv_input[i + 2] == 39) //if after & is invereted
			{
				g = inv_input[i + 1];
				g += "'";
			}
			else
				g = inv_input[i + 1];

			s = "wire" + to_string(++wire);

			
			d = last_source; 

			assign_mos(a, d, g, s, n, "nmos");
			trans.M.push_back(a);

			r.mos.push_back(a);

		}


		if (inv_input[i] == 124) //if |
		{
			count_or += 1;
			if ((count_or ==1))
			{
				//the one before
				if (inv_input[i - 1] == 39) //if before | is invereted
				{
					g = inv_input[i - 2];
					g += "'";
				}
				else
					g = inv_input[i - 1];

				n = "M" + to_string(++count);
				s = "wire" + to_string(++wire);
		
			    d = last_source;
				assign_mos(a, d, g, s, n, "nmos");
				trans.M.push_back(a);

				r.mos.push_back(a);

			}

			//the one after |

			if (inv_input[i + 2] == 39) //if after & is invereted
			{
				g = inv_input[i + 1];
				g += "'";
			}
			else
				g = inv_input[i + 1];

			n = "M" + to_string(++count);
			s = "wire" + to_string(++wire);

			d = last_source;

			assign_mos(a, d, g, s, n, "nmos");
			trans.M.push_back(a);
			r.mos.push_back(a);

		}

	}


	for (int i = 0; i < trans.inv.size(); i++)
		cout << trans.inv[i].name << " " << trans.inv[i].drain << " " << trans.inv[i].gate << " " << trans.inv[i].source << " " << trans.inv[i].body << " " << trans.inv[i].type << endl;


	if (blocks.size() > 0)
	{
		for (int i = 0; i < blocks[blocks.size() - 1].mos.size(); i++)
		{
			blocks[blocks.size() - 1].mos[i].source = "0"; //first drain
			blocks[blocks.size() - 1].mos[i].body = "0"; //first drain
		}


		for (int i = 0; i < blocks[0].mos.size(); i++)
			blocks[0].mos[i].drain = trans.firstD; //last source


	}
	for (int i = 0; i < blocks.size(); i++)
	{
		for (int j = 0; j < blocks[i].mos.size(); j++)
		{
			blocks[i].mos[j].drain = blocks[i].mos[0].drain;
			blocks[i].mos[j].source = blocks[i].mos[0].source;
			blocks[i].mos[j].body = blocks[i].mos[0].source;


			if(i>=1)
			blocks[i].mos[j].drain = blocks[i - 1].mos[0].source;
		}

	//	blocks[i].mos[blocks[i].mos.size() - 1].source = trans.lastS;
	}

	for (int i = 0; i < blocks.size(); i++)
		for (int j = 0; j < blocks[i].mos.size(); j++)
			cout << blocks[i].mos[j].name << " " << blocks[i].mos[j].drain << " " << blocks[i].mos[j].gate << " " << blocks[i].mos[j].source << " " << blocks[i].mos[j].body << " " << blocks[i].mos[j].type << endl;
}

void assign_mos(mosphet& a, string drain, string gate,string source, string name,string type )
{
	a.drain = drain;
	a.body = source;
	a.source = source;
	a.gate = gate;
	a.name = name;
	a.type = type;

}



void add_remove_bar(string a,int& count,int& wire)
{
	string A ;
	for (int i = 2; i < a.length(); i++)
	{
		if (isalpha(a[i]))
		{
			A += a[i];
			if (a[i + 1] == 39)
				i++;
			else
				A += "'";

		}
		else
			A += a[i];


	}

	PUN(A, a, count, wire);
	cout << endl;
	PDN(A, a, count, wire);
	cout << endl;

	
}

string anor(string a)
{
	string b;
    for (int i = 0; i < a.length(); i++)
	{
	if (a[i] == '&') b += '|';
	else
	if (a[i] == '|') b += '&';
	else
	b += a[i];
	}
	return b;
}
