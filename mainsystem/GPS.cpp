 #include <iostream>
 #include <string>
 #include <termios.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 #include <unistd.h>
 #include <sys/signal.h>
 #include <sys/types.h>
 #include <sys/ioctl.h>
 #include <fcntl.h>
 #include <errno.h>

 using namespace std;

 // int transfer_byte(int from, int to, int is_control);

 
 void print_status(int fd) 
 {
	 int status;
	 unsigned int arg;
	 status = ioctl(fd, TIOCMGET, &arg);
	 fprintf(stderr, "[STATUS]: ");
	 if(arg & TIOCM_RTS) fprintf(stderr, "RTS ");
	 if(arg & TIOCM_CTS) fprintf(stderr, "CTS ");
	 if(arg & TIOCM_DSR) fprintf(stderr, "DSR ");
	 if(arg & TIOCM_CAR) fprintf(stderr, "DCD ");
	 if(arg & TIOCM_DTR) fprintf(stderr, "DTR ");
	 if(arg & TIOCM_RNG) fprintf(stderr, "RI ");
	 fprintf(stderr, "\r\n");
 }
 

//int main()
void GPSreceiver()
//void GPSreceiver(double Lon, double Lat, double kmh, double direct)
 {
 	while(1)
 	{
 	int number=0;

	 int comfd;
 
	 struct termios oldtio, newtio;       //place for old and new port settings for serial port
 
	 struct termios oldkey, newkey;       //place tor old and new port settings for keyboard teletype
	 
//	 char *devicename = argv[1];
	 int need_exit = 0;
 
	 // speed_spec speeds[] =
		//  {
		// 	 {"1200", B1200},
		// 	 {"2400", B2400},
		// 	 {"4800", B4800},
		// 	 {"9600", B9600},
		// 	 {"19200", B19200},
		// 	 {"38400", B38400},
		// 	 {"57600", B57600},
		// 	 {"115200", B115200},
		// 	 {NULL, 0}
		//  };
	 // int speed = 9600;
 
	 // if(argc < 2) {
	 // 	fprintf(stderr, "example: %s /dev/ttyS0 [115200]\n", argv[0]);
	 // 	exit(1);
	 // }
 
 //	comfd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
	 comfd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
 //	comfd = open(devicename, O_RDWR | O_NOCTTY);
	 
	 // if (comfd < 0)
	 // {
	 // 	perror(devicename);
	 // 	exit(-1);
	 // }
 
	 // if(argc > 2) {	
	 // 	speed_spec *s;
	 // 	for(s = speeds; s->name; s++) {
	 // 		if(strcmp(s->name, argv[2]) == 0) {
	 // 			speed = s->flag;
	 // 			fprintf(stderr, "setting speed %s\n", s->name);
	 // 			break;
	 // 		}
	 // 	}
	 // }
 
	 fprintf(stderr, "C-a exit, C-x modem lines status\n");
 
	 // tcgetattr(STDIN_FILENO,&oldkey);
	 // newkey.c_cflag = B9600 | CRTSCTS | CS8 | CLOCAL | CREAD;
	 // newkey.c_iflag = IGNPAR;
	 // newkey.c_oflag = 0;
	 // newkey.c_lflag = 0;
	 // newkey.c_cc[VMIN]=1;
	 // newkey.c_cc[VTIME]=0;
	 // tcflush(STDIN_FILENO, TCIFLUSH);
	 // tcsetattr(STDIN_FILENO,TCSANOW,&newkey);
 
 
	 // tcgetattr(comfd,&oldtio); // save current port settings 
	 // newtio.c_cflag = speed | CS8 | CLOCAL | CREAD;
	 // newtio.c_iflag = IGNPAR;
	 // newtio.c_oflag = 0;
	 // newtio.c_lflag = 0;
	 // newtio.c_cc[VMIN]=1;
	 // newtio.c_cc[VTIME]=0;
	 // tcflush(comfd, TCIFLUSH);
 
 
	 // tcsetattr(comfd,TCSANOW,&newtio);
 
	 memset(&newtio , 0, sizeof(newtio));
 
	 newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	 newtio.c_iflag = IGNPAR;
	 newtio.c_oflag = 0;
	 newtio.c_lflag = 0;
	 newtio.c_cc[VMIN]=1;
	 newtio.c_cc[VTIME]=0;
 
	 tcflush(comfd,TCIFLUSH);
	 tcsetattr(comfd,TCSANOW,&newtio);
 
 
	 print_status(comfd);
 
 
  //    char buf[256];
 
	 // read(comfd,buf,256);
	 // printf("%s\n",buf);
 
	 // read(comfd,buf,256);
	 // printf("%s\n",buf);
 
 
	 //	unsigned char byte = {};
	 //	unsigned char* sta = new unsigned char[7];
	 
	 char byte[2] = {0,0};
	 char sta[7];
 
	 std::string sentence = "";
	 std::string oneSentence = "";
	 std::string zulutime = "";
	 std::string longitude = "";
	 std::string latitude = "";
	 std::string valid = "";
	 std::string speeed = "";
	 std::string direction = "";
 
	 double GMT = 0;
	 double Longitude = 0;
	 double Latitude = 0;
	 double Speeed = 0;
	 double Direction = 0;
	 
	 bool check = 0;


	 while (read(comfd,sta,6))
	 {
		 sta[6] = '\0';
		 for (int i = 0; i<6; i++)
			 sentence += sta[i];

	//	cout<<sentence<<endl;
	 //		if (!sentence.compare("$GNGGA"))
	 //		{	// 받아온 6개의 바이트와 비교, GNGGA인 경우
 
	 //			while (1)	// 한 문장 만드는 과정
	 //			{
	 //				serial.ReadByte(byte);
	 //		//		cout << byte;
 
	 //				if (byte == '\n')
	 //				{	// 문장의 끝을 찾는다.
	 //				//	serial.ReadByte(buff);	// 버퍼 비우고
	 //					oneSentence = sentence;
	 //					break;
	 //				}
	 //				else
	 //	//				cout << "더해" << endl;
 
	 //				sentence += byte;
	 //			}
	 //	//		cout << sentence << endl;
	 //			
 
	 //			int find_dot = sentence.find_first_of(",");
	 //			
	 //			int find_dot2 = sentence.find(",", find_dot + 1);
	 //			int find_dot3 = sentence.find(",", find_dot2 + 1);
	 //			int find_dot4 = sentence.find(",", find_dot3 + 1);
	 //			int find_dot5 = sentence.find(",", find_dot4 + 1);
	 //			int find_dot6 = sentence.find(",", find_dot5 + 1);
 
	 //			int find[7];
 
	 //			find[0] = sentence.find_first_of(",");
 
	 //			for (int i = 1; i < 8; i++)
	 //			{
	 //				find[i] = sentence.find(",", find[i - 1] + 1);
	 //			}
 
 
	 ////			cout << find_dot << endl << find_dot2 << endl << find_dot3 << endl << find_dot4 << endl << find_dot5 << endl << find_dot6 << endl;
 
	 ////		int index_long = sentence.find("N", 5) - 11;
	 ////			int index_lat = sentence.find("E", 5) - 12;
 
	 //			longitude = sentence.substr(find_dot2+1, 10);
	 //			latitude = sentence.substr(find_dot4+1, 11);
	 ////			latitude = sentence.substr(index_lat, 11);
	 //			zulutime = sentence.substr(find_dot+1, 9);
	 //			valid = sentence.substr(find_dot6 + 1, 1);
 
	 //			if (stoi(valid))
	 //			{
	 //				GMT = stod(zulutime) + 90000;	//	한국시간: 그리니치 표준시 +9
	 //				Longitude = stod(longitude);
	 //				Latitude = stod(latitude);
 
	 //				cout << fixed;
	 //				cout.precision(5);
	 //				cout << "GMT: " << GMT << "   Longitude: " << Longitude << "   Latitude: " << Latitude << endl;
	 //				cout << "-----------------------------------------------------------------" << endl;
	 //			}
	 //			else
	 //			{
	 //				cout << "invalid data 위성신호가 잡히지 않습니다" << endl;
	 //			}
	 //			
	 //		}
 
			 if(!sentence.compare("$GNRMC"))
			 {

				 while (1)	// 한 문장 만드는 과정
				 {
					 read(comfd,byte,1);
					 //		cout << byte;
 
					 if (!strcmp(byte,"\n"))
					 {	// 문장의 끝을 찾는다.
						 //	serial.ReadByte(buff);	// 버퍼 비우고
						 oneSentence = sentence;
						 
						 cout<<oneSentence<<endl;
						 
						 break;
					 }
					 else
						 sentence += byte;
				 }
				 //		cout << sentence << endl;
 
				 int find[8];
 
				 find[0] = sentence.find_first_of(",");			//구분점 찾아서 data 파싱
				 
				 for(int i = 1; i < 9; i++)
				 {
					 find[i] = sentence.find(",", find[i - 1] + 1);
				 }
 
				 zulutime = sentence.substr(find[0] + 1, 9);		//GMT
				 valid = sentence.substr(find[1] + 1, 1);		//GPS 신호 신뢰성(A = 신뢰할 수 있음, V = 신뢰할 수 없음)
				 latitude = sentence.substr(find[2] + 1, 10);	//위도
				 longitude = sentence.substr(find[4] + 1, 11);	//경도
				 speeed = sentence.substr(find[6] + 1, 5);		//Speed over ground(knots 단위 속도계)
				 direction = sentence.substr(find[7] + 1, 6);	//Track Angle in degree true(진행 방향 정북을 0~359도 까지 표현)
 				
			//	 cout<<"GMT : "<<zulutime<<endl<<"valid : "<<valid<<endl<<"lat : "<<latitude<<endl<<"lon : "<<longitude<<endl;

 				//printf("%s\n",latitude);

				 if (valid == "A")
				 {
				 //	string lat =latitude.substr(2, 8);
				 //	string lon = longitude.substr(3, 8);
 
				 //	double Lat = stod(lat) / 60;
				 //	double Lon = stod(lon) / 60;
 					
					 GMT = stod(zulutime) + 90000;	//	한국시간: 그리니치 표준시 +9
					 Latitude = stod(latitude);
					 Longitude = stod(longitude);
					 Speeed = stod(speeed)*1.852;		// knots -> km/h 변환
					 if (direction[0] != ',')
						 Direction = stod(direction);
 
/*				 	Lon = Longitude;
				 	Lat = Latitude;
 				    kmh = Speeed;
				    direct = Direction;
					
				    cout<<Lon<<endl<<Lat<<endl<<kmh<<endl<<direct<<endl;*/

					 //return;
 
					 printf("Current Time(hhmmss): %.0f\n",GMT);
					 printf("Longitude: %f\n",Longitude);
					 printf("latitude: %f\n",Latitude);
					 printf("Speed: %f\n",Speeed);
					 if(direction[0] != ',')
					 	printf("Direction: %f\n",Direction);


					 // cout << fixed;
					 // cout.precision(5);
					 // cout << "GMT: " << GMT 
					 // 	 << "   Longitude: " << Longitude 
					 // 	 << "   Latitude: " << Latitude
					 // 	 << endl
					 // 	 << "speed: " << Speed <<"km/h"
					 // 	 << "   Direction: " <<Direction
					 // 	 << endl;
					 //cout << "-----------------------------------------------------------------" << endl;
					 printf("-----------------------------\n");
					 return;
				 }
				 else
				 {
					 //cout << "invalid data 위성신호가 잡히지 않습니다" << endl;
					 printf("invalid data 위성신호가 잡히지 않습니다 Wait please\n");
					 return;
				 }
			 }
			 else
			 {
	 	//		cout << "GNGGA가 아닌 경우 SKIP" << endl;
				 //	sentence.clear();

				 while (1)	//txt 비우는 과정
				 {
					 //serial.ReadByte(byte);
					 read(comfd,byte,1);
	 			//	cout <<"one byte :: " << byte <<"****"<<endl;
 					
					 if (!strcmp(byte,"\n"))
					 {
						 //			serial.ReadByte(buff);	// 버퍼 비우고
						 oneSentence = sentence;
					//	 cout<<"own sentence: "<<oneSentence<<endl;
						 break;
					 }
					 else
				 //		cout << "더해" << endl;
 
						sentence += byte;
						// number++;
						// if(number >100)
						// {
						// 	cout<<sentence<<endl;
						// 	return;
						// }
				//	cout<<byte<<endl;
				 }
		 //		cout << sentence << endl;
 			 sentence.clear();

			 }
 

	 //		cout << oneSentence << endl;
		 }
 
 
 
		close(comfd);
		 

		}
		return;
	}
 
 
 
 
 
	 // while(!need_exit) {
	 // 	fd_set fds;
	 // 	int ret;
		 
	 // 	FD_ZERO(&fds);
	 // 	FD_SET(STDIN_FILENO, &fds);
	 // 	FD_SET(comfd, &fds);
 
 
	 // 	ret = select(comfd+1, &fds, NULL, NULL, NULL);
	 // 	if(ret == -1) {
	 // 		perror("select");
	 // 	} else if (ret > 0) {
	 // 		if(FD_ISSET(STDIN_FILENO, &fds)) {
	 // 			need_exit = transfer_byte(STDIN_FILENO, comfd, 1);
	 // 		}
	 // 		if(FD_ISSET(comfd, &fds)) {
	 // 			need_exit = transfer_byte(comfd, STDIN_FILENO, 0);
	 // 		}
	 // 	}
	 // }
 
	 // tcsetattr(comfd,TCSANOW,&oldtio);
	 // tcsetattr(STDIN_FILENO,TCSANOW,&oldkey);

 
 // int transfer_byte(int from, int to, int is_control) {
	//  char c;
	//  int ret;
	//  do {
	// 	 ret = read(from, &c, 1);
	//  } while (ret < 0 && errno == EINTR);
	//  if(ret == 1) {
	// 	 if(is_control) {
	// 		 if(c == '\x01') { // C-a
	// 			 return -1;
	// 		 } else if(c == '\x18') { // C-x
	// 			 print_status(to);
	// 			 return 0;
	// 		 }
	// 	 }
	// 	 while(write(to, &c, 1) == -1) {
	// 		 if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
	// 	 }
	//  } else {
	// 	 fprintf(stderr, "\nnothing to read. probably port disconnected.\n");
	// 	 return -2;
	//  }
	//  return 0;
 // }
 
 
 // 
