#ifndef GIORNOORARIO_H
#define GIORNOORARIO_H

class orario {
	private:
		int _ore;
		int _minuti;
		
	public:
		orario();
		orario(int, int);
		int getOra() const { return _ore; };
		int getMinuti() const { return _minuti; };
		bool operator>(const orario&);
		orario &operator=(const orario&);
		orario &operator+(const int);
};

#endif
