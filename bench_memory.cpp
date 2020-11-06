#include<iostream>
#include <cstring>
#include <ctime>
#include <fstream>
#include <math.h>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <time.h>

using namespace std;

enum type_memory {
	RAM,
	HDD,
	SSD,
	flash,
};

#define BLOCK_SIZE 1024

long int max(long int * arr, int N) {
	long int max = arr[0];
	for(int i = 0; i < N; i++) {
		if(arr[i] > max) { 
			max = arr[i];
		}
	}
	return max;
}

long int min(long int * arr, int N) {
	long int min = arr[0];
	for(int i = 0; i < N; i++) {
		if(arr[i] < min) { 
			min = arr[i];
		}
	}
	return min;
}
void wall() {
	for(int i = 0; i < 10; i++) {
		cout << "***";
	}
	cout << endl;
}

string typeMem(int type) {
	switch(type) {
		case RAM: return "RAM"; 
		case HDD: return "HDD"; 
		case SSD: return "SSD"; 
		case flash: return "flash";
		default: wall(); return "cheburashka\n"; 
	}
}
void check_memory(int argc, char *argv[], int &memory_type) {
	if(argc <= 1) {
		cout << "Memory type: RAM\n";
		memory_type = RAM;
		return;
	}
	int key_index = -1;
	for(int i = 1; i < argc; i++) {

		if(strcmp(argv[i], "-m") == 0) {
			key_index = i;
		}
	}

	if(key_index == -1) {
		cout << "Memory type: RAM\n";
		memory_type = RAM;
		return;
	}

	if(strcmp(argv[key_index + 1], "RAM") == 0) {
		memory_type  = RAM;
		//cout << "Memory Type: RAM" << endl;
	} 
	else if(strcmp(argv[key_index + 1], "HDD") == 0) {
		memory_type  = HDD;
		//cout << "Memory Type: HDD\n";
	}
	else if(strcmp(argv[key_index + 1], "SSD") == 0) {
		memory_type  = SSD;
		//cout << "Memory Type: SSD\n";
	}
	else if(strcmp(argv[key_index + 1], "flash") == 0) {
		memory_type  = flash;
		//cout << "Memory Type: flash\n";
	}
	else {
		cout << "\nPlease enter the correct memory type\n";
		exit(-1);
	}
}

void check_block_size(int argc, char *argv[], int &block_size) {
	int key_index = -1;

	if(argc <= 1) {
		cout << "Block size:" << block_size << "Kb\n";
		return;
	}

	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-b") == 0) {
			key_index = i;//запомнили расположение ключа -b
		}
	}
	
	if(key_index == -1) {
		cout << "Block size:" << block_size << "Kb\n";
		block_size = BLOCK_SIZE;
		return;
	}

	int lenth_argv_memory = strlen(argv[key_index+1]);

	block_size = 0;
	for(int i = 0; i <= lenth_argv_memory; i++) {
		//cout << argv[key_index + 1][i] << endl;
		if(isdigit(argv[key_index + 1][i])) {
			block_size = block_size * 10 + ((argv[key_index + 1][i]) - '0');
		}
	}
	
	if(argv[key_index + 1][lenth_argv_memory - 2] == 'M' &&
		argv[key_index + 1][lenth_argv_memory - 1] == 'b') {
		block_size *= BLOCK_SIZE;
}
}

void check_launch_count(int argc, char *argv[], int &launch_count) {
	int key_index = -1;
	if(argc <= 1) {
		cout << "Launch count:" << launch_count << "Kb\n";
		return;
	}
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-l") == 0) {
			key_index = i;//запомнили расположение ключа -l
		}
	}
	if(key_index == -1) {
		cout << "Launch count:" << launch_count << "Kb\n";
		return;
	}
	int lenth_argv_launch_count = strlen(argv[key_index+1]);
	launch_count = 0;
	for(int i = 0; i <= lenth_argv_launch_count; i++) {
		if(isdigit(argv[key_index + 1][i])) {
			launch_count = launch_count * 10 + ((argv[key_index + 1][i]) - '0');
		}
	}
}

void benchmark_RAM(int memory_type, int block_size, int launch_count) {
	struct timespec start_write, stop_write;
	struct timespec start_read, stop_read;

	long double relative_write = 0;
	long double absolute_write = 0;

	long double relative_read = 0;
	long	double absolute_read = 0;
	
	long double result_sum = 0;

	double result_sum_read = 0;

	long int *result = new long int[launch_count];
	long int *result_read = new long int[launch_count];

	int temp = 0;
	int array_size = block_size / sizeof(int);

	for(int i = 0; i < launch_count; i++) {
		double * arr = new double[array_size];
		
		clock_gettime(CLOCK_REALTIME, &start_write);
		for(int j = 0; j < array_size; j++) {
			arr[j] = rand();
		}
		clock_gettime(CLOCK_REALTIME, &stop_write);

		clock_gettime(CLOCK_REALTIME, &start_read);
		for(int j = 0; j < array_size; j++) {
			temp = arr[j];
		}
		clock_gettime(CLOCK_REALTIME, &stop_read);
		delete[] arr;
		
		result[i] = (stop_write.tv_sec - start_write.tv_sec)+(stop_write.tv_nsec - start_write.tv_nsec);
		result_sum += result[i];//запись

		result_read[i] = (stop_read.tv_sec - start_read.tv_sec)+(stop_read.tv_nsec - start_read.tv_nsec);
		result_sum_read += result_read[i];//чтение


	}

	long int min_write = min(result, launch_count);
	long int max_write = max(result, launch_count);

	int max_read = max(result_read, launch_count);
	int min_read = min(result_read, launch_count);


	result_sum /= launch_count;
	result_sum_read /= launch_count;


	absolute_write = (max_write - min_write) / block_size;
	relative_write = absolute_write / result_sum;

	absolute_read = ((long double)max_read - (long double)min_read) / (long double)block_size;
	relative_read = absolute_read / result_sum_read;

	ofstream benchmark_output;
	benchmark_output.open("bench_memory.csv", ios_base::app);

	benchmark_output << "Memory type;Blocksize;ElementType;BufferSize;LaunchNum;Timer;";
	benchmark_output << "WriteTime;AverageWriteTime;WriteBandwidth;AbsError(write);RelError(write);";
	benchmark_output << "ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);";
	benchmark_output << endl;
	for(int i = 0; i < launch_count; i++) {
	//wall();

		benchmark_output <<  typeMem(RAM)  << ";" << block_size << ";"  << typeid(result).name() << ";";
		benchmark_output << sizeof(int)<< ";"  << i << ";" << "clock_gettime;";

		benchmark_output << result[i] << ";" << result_sum << ";" << (block_size/result_sum) * 1000000 << ";";
		benchmark_output << fixed <<  absolute_write<< ";"  << fixed << relative_write<< ";";

		benchmark_output << result_read[i]<< ";" << result_sum_read<< ";"  << (block_size/result_sum_read) * 1000000<< ";"  << 
		fixed << absolute_read<< ";" << fixed << relative_read<< ";";
		benchmark_output << endl;
	}
	benchmark_output.close();
}

void benchmark_HDD(int memory_type, int block_size, int launch_count) {
	struct timespec start_write, stop_write;
	struct timespec start_read, stop_read;

	long double relative_write = 0;
	long double absolute_write = 0;

	long double relative_read = 0;
	long	double absolute_read = 0;
	
	long double result_sum = 0;

	double result_sum_read = 0;

	long int *result = new long int[launch_count];//массив времени

	long int *result_read = new long int[launch_count];

	int temp = 0;
	int array_size = block_size / sizeof(int);
	for(int i = 0; i < launch_count; i++) {//цикл испытаний
		double * arr = new double[array_size];
		for(int j = 0; j < array_size; j++) {
			arr[j] = rand();
		}
		ofstream write;
		write.open("../../test.txt");
		clock_gettime(CLOCK_REALTIME, &start_write);
		for(int j = 0; j < array_size; j++) {
			write << arr[j] << endl;
		}
		clock_gettime(CLOCK_REALTIME, &stop_write);
		write.close();

		ifstream read;
		read.open("../../test.txt");
		clock_gettime(CLOCK_REALTIME, &start_read);
		for(int j = 0; j < array_size; j++) {
			read >> arr[j];
		}
		clock_gettime(CLOCK_REALTIME, &stop_read);
		read.close();
		delete[] arr;

		result[i] = (stop_write.tv_sec - start_write.tv_sec)+(stop_write.tv_nsec - start_write.tv_nsec);
		result_sum += result[i];//запись

		result_read[i] = (stop_read.tv_sec - start_read.tv_sec)+(stop_read.tv_nsec - start_read.tv_nsec);
		result_sum_read += result_read[i];//чтение


	}

	cout << "RESULT WRITE: \n";
	for(int i = 0; i < launch_count; i++) {
		cout << result[i] << " ";
	}
	cout << endl << "\nRESULT READ: \n";
	for(int i = 0; i < launch_count; i++) {
		cout << result_read[i] << " ";
	}
	cout << endl;


	long int min_write = min(result, launch_count);
	long int max_write = max(result, launch_count);

	int max_read = max(result_read, launch_count);
	int min_read = min(result_read, launch_count);

	cout << endl;

	result_sum /= launch_count;
	result_sum_read /= launch_count;

	absolute_write = (max_write - min_write) / block_size;
	relative_write = absolute_write / result_sum;

	absolute_read = ((long double)max_read - (long double)min_read) / (long double)block_size;
	relative_read = absolute_read / result_sum_read;


	ofstream benchmark_output;
	benchmark_output.open("bench_memory.csv", ios_base::app);
	benchmark_output << "Memory type;Blocksize;ElementType;BufferSize;LaunchNum;Timer;";
	benchmark_output << "WriteTime;AverageWriteTime;WriteBandwidth;AbsError(write);RelError(write);";
	benchmark_output << "ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);";
	benchmark_output << endl;
	for(int i = 0; i < launch_count; i++) {
	//wall();

		benchmark_output <<  typeMem(RAM)  << ";" << block_size << ";"  << typeid(result).name() << ";";
		benchmark_output << sizeof(int)<< ";"  << i << ";" << "clock_gettime;";

		benchmark_output << result[i] << ";" << result_sum << ";" << (block_size/result_sum) * 1000000 << ";";
		benchmark_output << fixed <<  absolute_write<< ";"  << fixed << relative_write<< ";";

		benchmark_output << result_read[i]<< ";" << result_sum_read<< ";"  << (block_size/result_sum_read) * 1000000<< ";"  << 
		fixed << absolute_read<< ";" << fixed << relative_read<< ";";
		benchmark_output << endl;

	}
	benchmark_output.close();
}


void benchmark_SSD(int memory_type, int block_size, int launch_count) {
	struct timespec start_write, stop_write;
	struct timespec start_read, stop_read;

	long double relative_write = 0;
	long double absolute_write = 0;

	long double relative_read = 0;
	long	double absolute_read = 0;
	
	long double result_sum = 0;

	double result_sum_read = 0;

	long int *result = new long int[launch_count];//массив времени

	long int *result_read = new long int[launch_count];

	int temp = 0;
	int array_size = block_size / sizeof(int);
	for(int i = 0; i < launch_count; i++) {//цикл испытаний
		double * arr = new double[array_size];
		for(int j = 0; j < array_size; j++) {
			arr[j] = rand();
		}
		ofstream write;

		write.open("../../lab2/test.txt");
		clock_gettime(CLOCK_REALTIME, &start_write);
		for(int j = 0; j < array_size; j++) {
			write << arr[j] << endl;
		}
		clock_gettime(CLOCK_REALTIME, &stop_write);
		write.close();

		ifstream read;
		read.open("../../lab2/test.txt");
		clock_gettime(CLOCK_REALTIME, &start_read);
		for(int j = 0; j < array_size; j++) {
			read >> arr[j];
		}
		clock_gettime(CLOCK_REALTIME, &stop_read);
		read.close();
		delete[] arr;

		result[i] = (stop_write.tv_sec - start_write.tv_sec)+(stop_write.tv_nsec - start_write.tv_nsec);
		result_sum += result[i];//запись

		result_read[i] = (stop_read.tv_sec - start_read.tv_sec)+(stop_read.tv_nsec - start_read.tv_nsec);
		result_sum_read += result_read[i];//чтение


	}

	cout << "RESULT WRITE: \n";
	for(int i = 0; i < launch_count; i++) {
		cout << result[i] << " ";
	}
	cout << endl << "\nRESULT READ: \n";
	for(int i = 0; i < launch_count; i++) {
		cout << result_read[i] << " ";
	}
	cout << endl;


	long int min_write = min(result, launch_count);
	long int max_write = max(result, launch_count);

	int max_read = max(result_read, launch_count);
	int min_read = min(result_read, launch_count);

	result_sum /= launch_count;
	result_sum_read /= launch_count;

	absolute_write = (max_write - min_write) / block_size;
	relative_write = absolute_write / result_sum;

	absolute_read = ((long double)max_read - (long double)min_read) / (long double)block_size;
	relative_read = absolute_read / result_sum_read;

	ofstream benchmark_output;
	benchmark_output.open("bench_memory.csv", ios_base::app);
	benchmark_output << "Memory type;Blocksize;ElementType;BufferSize;LaunchNum;Timer;";
	benchmark_output << "WriteTime;AverageWriteTime;WriteBandwidth;AbsError(write);RelError(write);";
	benchmark_output << "ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);";
	benchmark_output << endl;
	for(int i = 0; i < launch_count; i++) {
	//wall();

		benchmark_output <<  typeMem(RAM)  << ";" << block_size << ";"  << typeid(result).name() << ";";
		benchmark_output << sizeof(int)<< ";"  << i << ";" << "clock_gettime;";

		benchmark_output << result[i] << ";" << result_sum << ";" << (block_size/result_sum) * 1000000 << ";";
		benchmark_output << fixed <<  absolute_write<< ";"  << fixed << relative_write<< ";";

		benchmark_output << result_read[i]<< ";" << result_sum_read<< ";"  << (block_size/result_sum_read) * 1000000<< ";"  << 
		fixed << absolute_read<< ";" << fixed << relative_read<< ";";
		benchmark_output << endl;

	}
	benchmark_output.close();
}

void benchmark_flash(int memory_type, int block_size, int launch_count) {
	struct timespec start_write, stop_write;
	struct timespec start_read, stop_read;

	long double relative_write = 0;
	long double absolute_write = 0;

	long double relative_read = 0;
	long	double absolute_read = 0;
	
	long double result_sum = 0;

	double result_sum_read = 0;

	long int *result = new long int[launch_count];//массив времени

	long int *result_read = new long int[launch_count];

	int temp = 0;
	int array_size = block_size / sizeof(int);
	//cout <<"Array size: " << array_size << endl;
	for(int i = 0; i < launch_count; i++) {//цикл испытаний
		double * arr = new double[array_size];
		for(int j = 0; j < array_size; j++) {
			arr[j] = rand();
		}
		ofstream write;

		write.open("../../lab2/test.txt");
		clock_gettime(CLOCK_REALTIME, &start_write);
		for(int j = 0; j < array_size; j++) {
			write << arr[j] << endl;
		}
		clock_gettime(CLOCK_REALTIME, &stop_write);
		write.close();

		ifstream read;
		read.open("../../lab2/test.txt");
		clock_gettime(CLOCK_REALTIME, &start_read);
		for(int j = 0; j < array_size; j++) {
			read >> arr[j];
		}
		clock_gettime(CLOCK_REALTIME, &stop_read);
		read.close();
		delete[] arr;

		result[i] = (stop_write.tv_sec - start_write.tv_sec)+(stop_write.tv_nsec - start_write.tv_nsec);
		result_sum += result[i];//запись

		result_read[i] = (stop_read.tv_sec - start_read.tv_sec)+(stop_read.tv_nsec - start_read.tv_nsec);
		result_sum_read += result_read[i];//чтение


	}

	long int min_write = min(result, launch_count);
	long int max_write = max(result, launch_count);

	int max_read = max(result_read, launch_count);
	int min_read = min(result_read, launch_count);

	cout << endl;

	result_sum /= launch_count;
	result_sum_read /= launch_count;

	absolute_write = (max_write - min_write) / block_size;
	relative_write = absolute_write / result_sum;

	absolute_read = ((long double)max_read - (long double)min_read) / (long double)block_size;
	relative_read = absolute_read / result_sum_read;

	ofstream benchmark_output;
	benchmark_output.open("bench_memory.csv", ios_base::app);
	benchmark_output << "Memory type;Blocksize;ElementType;BufferSize;LaunchNum;Timer;";
	benchmark_output << "WriteTime;AverageWriteTime;WriteBandwidth;AbsError(write);RelError(write);";
	benchmark_output << "ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);";

	for(int i = 0; i < launch_count; i++) {
		benchmark_output <<  typeMem(RAM)  << ";" << block_size << ";"  << typeid(result).name() << ";";
		benchmark_output << sizeof(int)<< ";"  << i << ";" << "clock_gettime;";

		benchmark_output << result[i] << ";" << result_sum << ";" << (block_size/result_sum) * 1000000 << ";";
		benchmark_output << fixed <<  absolute_write<< ";"  << fixed << relative_write<< ";";

		benchmark_output << result_read[i]<< ";" << result_sum_read<< ";"  << (block_size/result_sum_read) * 1000000<< ";"  << 
		fixed << absolute_read<< ";" << fixed << relative_read<< ";";
		benchmark_output << endl;

		benchmark_output.close();
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int memory_type = RAM;
	int block_size = 1024;//Kb
	int launch_count = 10;

	wall();
	/*ПРОВЕРКА НА КЛЮЧИ*/

	//ПЕРВЫЙ КЛЮЧ НА ТИП ПАМЯТИ
	check_memory(argc, argv, memory_type);
	//ВТОРОЙ КЛЮЧ НА РАЗМЕР БЛОКА
	check_block_size(argc, argv, block_size);
	//cout << "Block size: " << block_size << endl;
	//ТРЕТИЙ КЛЮЧ НА КОЛИЧЕСТВО ИТЕРАЦИЙ
	check_launch_count(argc, argv, launch_count);
	//cout << "Launch count: " << launch_count << endl;

	/*ПОЕХАЛИ!*/
	/**/
	switch(memory_type) {
		case RAM: 	benchmark_RAM(memory_type, block_size, launch_count); break;
		case HDD: 	benchmark_HDD(memory_type, block_size, launch_count); break;
		case SSD: 	benchmark_SSD(memory_type, block_size, launch_count); break;
		case flash: 	benchmark_flash(memory_type, block_size, launch_count); break;
		default: wall(); cout << "\ni'm a chupakabra\n"; wall();
	}
	return 0;
}