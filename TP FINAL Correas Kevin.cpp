#include <iostream>
#include <conio2.h>
#include <ctime>
#include <windows.h>

using namespace std;

class Camioneta{
protected:
	//Posición 
	int x;
	int y;
	
	//puntaje
	int puntaje;
	
	
	//Tiempo
	int velocidad;
	clock_t tempo;
	clock_t paso;
	
	//Arreglo auto
	char camio[6][3];
	int color;//COLOR DE LOS AUTOS
	char obstaculos[3][3];
	int ancho;
	int alto;
	
public:
	Camioneta(int vel,int col);
	virtual void dibujar();
	virtual void borrar();
	void score();
};

Camioneta::Camioneta(int vel,int col){
	//Tiempo
	velocidad=vel;
	paso=CLOCKS_PER_SEC/velocidad;
	tempo=clock();
	color=col;
	
	//puntaje
	puntaje=0;
	
	//diseño de la camio
	//Seccion arriba
	camio[0][0]='[';
	camio[1][0]=']';
	camio[2][0]=' ';
	camio[3][0]=' ';
	camio[4][0]='[';
	camio[5][0]=']';
	
	//Seccion medio
	camio[0][1]=' ';
	camio[1][1]=' ';
	camio[2][1]='[';
	camio[3][1]=']';
	camio[4][1]=' ';
	camio[5][1]=' ';
	
	//Seccion abajo
	camio[0][2]='[';
	camio[1][2]=']';
	camio[2][2]=' ';
	camio[3][2]=' ';
	camio[4][2]='[';
	camio[5][2]=']';
	
	//parte superior del obstaculo
	obstaculos[0][0]=' ';
	obstaculos[1][0]='O';
	obstaculos[2][0]=' ';
	
	//Seccion medio del obstaculo
	obstaculos[0][1]='-';
	obstaculos[1][1]='|';
	obstaculos[2][1]='-';
	
	//Seccion inferior del obstaculo
	obstaculos[0][2]='(';
	obstaculos[1][2]=' ';
	obstaculos[2][2]=')';
}
void Camioneta::dibujar(){
	for(int i=0;i<ancho;i++){
		for(int k=0;k<alto;k++){
			textcolor(color);
			putchxy(x+i,y+k,camio[i][k]);
			
		}
	}
}

void Camioneta::borrar(){
	for (int i= -1; i<ancho+1; i++){
		for (int k= -1; k<alto+1; k++){
			textcolor(color);
			putchxy(x+i,y+k,' ');
			
		}
	}
}

void Camioneta::score(){
	puntaje++;
	putchxy(52,2,' ');cout<<puntaje;
}

//******************************************************************************
//Generamos la clase camioneta jugador
class CamionetaJugador:public Camioneta{
private:
	// el offset es la velocidad de salto en la pantalla
	int offsert;
	
public:
	CamionetaJugador(int x0,int y0,int vel,int colo);
	
	//En este caso el update recibe una instruccion 
	// 1: izquierda
	// 2 derecha
	bool update(int);
};

CamionetaJugador::CamionetaJugador(int x0,int y0,int vel,int colo):Camioneta(vel,colo){
	x=x0;
	y=y0;
	offsert=2;
	//ancho y alto de la camio
	ancho=6;
	alto=3;
	
	dibujar();
}

//Update de la camioneta del jugador
bool CamionetaJugador::update(int v){
	//Primero se borra
	borrar();
	
	switch(v){
	case 1: { // izqierda
		if(x>4){//MARCANDO LIMITE IZQUIERDO DEL MAPA
			x = x - 1*(offsert);
		}
		break;} 
	case 2: { // derecha
		if(x<48){//MARCANDO LIMITE DERECHO DEL MAPA
			x = x + 1*(offsert);
		}
		
		break;} 
	}
	
	
	// que no pase la posicion 52 (tome esto ya que el auto se va a encontrar en el medio) de x
	// ni baje de la posicion 0
	if(x < 52 && x > 0){
		
		dibujar();
		
		return true;
	}
	
	else{return false;};
	
}
//*****************************************************************************
class CamionetaEnemiga:public Camioneta{
	//ponemos constructor de camioneta enemiga y su update
public:
	CamionetaEnemiga(int x0,int y0,int vel,int colo);
	bool update();
};

CamionetaEnemiga::CamionetaEnemiga(int x0,int y0,int vel,int colo): Camioneta(vel,colo){
	x=x0;
	y=y0;

	ancho=6;
	alto=3;
	
}
bool CamionetaEnemiga::update(){
	/* mientras no llegue 20
	hicimos que el auto vuelva a su posicion original
	*/
	if(y<20){//limite inferior del mapa
		if(tempo+paso<clock()){
			// borro
			borrar();
			// muevo
			y++;
			// dibujo
			dibujar();
			tempo=clock();
		}
	}else if(y==20){ //respawn
		//borro la ultima posicion
		borrar();
		//pongo nueva coordenada
		
		y=5;
		//Dibujo
		score();
		dibujar();
	}return true;
	
}

//*****************************************************************************
class CamionetaEnemiga2:public Camioneta{
	//ponemos constructor de camioneta enemiga y su update
public:
	CamionetaEnemiga2(int x0,int y0,int vel,int colo);
	bool update();
};
CamionetaEnemiga2::CamionetaEnemiga2(int x0,int y0,int vel,int colo):Camioneta(vel,colo){
	x=x0;
	y=y0;
	
	ancho=6;
	alto=3;
	
}
bool CamionetaEnemiga2::update(){
	
	if(y<13){//limite para girar
		if(tempo+paso<clock()){
			// borro
			borrar();
			// muevo
			y++;
			x--;//TRATAREMOS DE QUE EMPIECE A ZIGZAGUEAR
			// dibujo
			dibujar();
			tempo=clock();
		}
	}
	if(y>=13){//limite para girar
		if(tempo+paso<clock()){
			// borro
			borrar();
			// muevo
			y++;
			x++;//TRATAREMOS DE QUE EMPIECE A ZIGZAGUEAR
			// dibujo
			
			dibujar();
			tempo=clock();
		}
	}
	
	//RESPAWN
	if(y==20){
		//borro la ultima posicion
		borrar();
		//pongo nueva coordenada
		x=38;
		y=5;
		//Dibujo
		score();
		dibujar();
	}return true;
}

//*****************************************************************************

class Obstaculo:public Camioneta{
public:
	Obstaculo(int x0,int y0,int vel,int colo);
	bool update();
	void dibujar();
	void borrar();
};
Obstaculo::Obstaculo(int x0,int y0,int vel,int colo):Camioneta(vel,colo){
	x=x0;
	y=y0;

	ancho=3;
	alto=3;
}
void Obstaculo::dibujar(){
	for(int i=0;i<ancho;i++){
		for(int k=0;k<alto;k++){
			textcolor(GREEN);
			putchxy(x+i,y+k,obstaculos[i][k]);
			
		}
	}
}
void Obstaculo::borrar(){
	for (int i= -1; i<ancho+1; i++){
		for (int k= -1; k<alto+1; k++){
			textcolor(GREEN);
			putchxy(x+i,y+k,' '); 
		}
	}
}
bool Obstaculo::update(){
	// mientras no llegue 20
	if(y<20){//limite inferior del mapa
		if(tempo+paso<clock()){
			// borro
			borrar();
			// muevo
			y++;
			// dibujo
			dibujar();
			tempo=clock();
		}
	}else if(y>=20){
		//borro la ultima posicion
		borrar();
		
		y=5;
		//Dibujo
		dibujar();
	}return true;
}
//*****************************************************************************

class Juego{
	
	CamionetaJugador *camioJug = new CamionetaJugador(20,20,3,7);//posicion,velocidad y color
	CamionetaEnemiga *camioEne1 = new CamionetaEnemiga(11,5,7,14);//posicion,velocidad y color
	CamionetaEnemiga2 *camioEne2= new CamionetaEnemiga2(38,5,11,4);//posicion,velocidad y color
	Obstaculo *obst1= new Obstaculo(24,5,2,2);//posicion,velocidad y color
	
public:
	Juego(){
	};
	void play();
	void limites();
};

void Juego::play(){
	/**
	aca de debe ir una condicion de corte que finalice el juego
	en un juego con interacciones se suele hacer un while true y corta
	alguna accion en el juego: vida = 0; algujna tecla, etc.
	En este ejemplo el metodo update dira cuando cortar cuando
	el contador llegue a 0 lanzara un false
	**/
	bool flag = true;
	while(flag) {
		camioEne1->update();
		camioEne2->update();
		obst1->update();
		
		if(kbhit()) { 
			int tecla=getch();
			
			switch(tecla)
			{
			case 75: //izquierda
				flag = camioJug->update(1);
				break;
			case 77: //derecha
				flag = camioJug->update(2);
				break;
			}
		}
	}
}

void Juego::limites(){
	/*la idea de este es tratar de marcar los limites del mapa
	y tambien vamos aprovechar a dejar escrito
	los controles para poder manejar el auto	
	*/
	
	//Limite Horizontal
	for(int i=2;i<56;i++){
		putchxy(i,3,'=');
		putchxy(i,24,'=');
	}
	
	//Limite vertical
	for(int i=4;i<24;i++){
		putchxy(2,i,'|');
		putchxy(55,i,'|');
	}
	
	/*
	No se me ocurrio otra forma
	de mostrar los controles en ese sector de pantalla
	controlarlo despues
	*/
	putchxy(1,2,' ');cout<<"Se controla con las flechas <- ->";
	//mostrar puntaje
	putchxy(40,2,' ');cout<<"Puntaje: ";
	
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	Juego *J= new Juego;
	J->limites();
	J->play();
	return 0;
}


