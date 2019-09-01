#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "snap7.h"
#include "s7.h"
#include <string>
#include <time.h>
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <math.h>
#include <SFML/Window/Mouse.hpp>
#include "pid.h"
#include "src/particlesystem.cpp"
using namespace std;

byte MyDB100[50]; // byte is a portable type of snap7.h
byte MyDB101[30]; // byte is a portable type of snap7.h
TS7Client *MyClient;

void SimplyGet() {

MyClient = new TS7Client();
MyClient->ConnectTo("172.16.99.77",0,2);
MyClient->DBRead(100, 0, 46, &MyDB100);
MyClient->DBWrite(101, 0, 30, &MyDB101);

delete MyClient;
}

float runderern(float value, int to)
{
        float places = pow(10.0, to);
        return round(value * places) / places;
}

string runden(float wert, int stellen){

    string text = to_string(wert);
    text = text.substr(0,stellen);

return text;

}

int main()
{


sf::Vector2u picsize;
sf::Image graph;
picsize.x = 1920;
picsize.y = 1080;
graph.create(picsize.x, picsize.y, sf::Color(0, 00, 0));
// y = 2x
for (unsigned int x = 0; x < picsize.x; x++)
{
    unsigned int y = 2u * x * sin(x);
    if (y < picsize.y)
    {
        graph.setPixel(x, y, sf::Color(255, 0, 0));
        graph.setPixel(x+2, y+2, sf::Color(255, 0, 0));
        graph.setPixel(x+1, y+1, sf::Color(255, 0, 0));

    }
}

sf::Texture Diagramm;
Diagramm.loadFromImage(graph);
sf::Sprite Diagrammsprite(Diagramm);
    ParticleSystem particles(1000);
    PID pid_x = PID(0.1, 100, -100, 0.1, 0.01, 0.5); // PID-Regler
    PID pid_y = PID(0.1, 100, -100, 0.1, 0.01, 0.5); // PID-Regler

    clock_t start, ende;
    sf::Clock Clock;
    long diff;
    double tmp;
    double val = 20;
    start = clock();

    // SimplyGet();
    float zahl1 = S7_GetRealAt(MyDB100,0);
    float zahl2 = S7_GetRealAt(MyDB100,4);
    float zahl3 = S7_GetRealAt(MyDB100,8);
    float zahl4 = S7_GetRealAt(MyDB100,12);
    float IW_VerdDruck = S7_GetRealAt(MyDB100,28);
    int   zahl5 = S7_GetIntAt (MyDB100,44);

    int x=0;
    int y=0;
    float p=1.f;
    float v=20.0f;


    for (int i = 0; i < 100; i++) {
        double inc = pid_x.calculate(0, val);
        printf("val:% 7.3f inc:% 7.3f\n", val, inc);
        val += inc;
}


    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1920, 1080), "Snap7");

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("Quantum.bmp"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);

    sf::Music music;
    if (!music.openFromFile("heaven.ogg"))
    return EXIT_FAILURE;

        music.play();

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return EXIT_FAILURE;
    //sf::Text text("Hello SFML", font, 50);
    std::string zeig = to_string(zahl1)+to_string(zahl2);
    sf::Text text (zeig,font,50);

    //sf::Text text(ss, font, 50);
	// Start the game loop
	ende = clock();
	diff=ende - start;
//	cout << "benötigte Ticks: " << diff << endl;
//	cout << "benötigte Zeit: " << diff/CLOCKS_PER_SEC << endl;
//    cout << CLOCKS_PER_SEC << endl;


    while (app.isOpen())
    {
        sf::Time elapsed1 = Clock.getElapsedTime();
        float ElapsedTime = elapsed1.asSeconds();
        Clock.restart();
        start = clock();
        S7_SetByteAt(MyDB101,0,0);

        sf::Vector2i position = sf::Mouse::getPosition(app);

        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
          S7_SetBitAt(MyDB101,0,0,1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
          S7_SetBitAt(MyDB101,0,1,1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
          S7_SetBitAt(MyDB101,0,2,1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
          S7_SetBitAt(MyDB101,0,3,1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        {
          S7_SetBitAt(MyDB101,0,4,1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
        if(x>0){x--;}
        if(v>0){v-=1.f* ElapsedTime;}
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
        if(x<800){x++;}
        if(v<100){v+=1.f* ElapsedTime;;}
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
        if(y>0){y--;}
        if(p>0.1){p-=0.01f* ElapsedTime;;}
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
        if(y<600){y++;}
        if(p<2.1){p+=0.01f* ElapsedTime;;}
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {

        S7_SetBitAt(MyDB101,0,1,1);

        // quit...
        }

       // SimplyGet();
        zahl1 = S7_GetRealAt(MyDB100,0);
        zahl2 = S7_GetRealAt(MyDB100,4);
        zahl3 = S7_GetRealAt(MyDB100,8);
        zahl4 = S7_GetRealAt(MyDB100,12);
        IW_VerdDruck = S7_GetRealAt(MyDB100,24);
        string Text_IW_VerdDruck = runden(IW_VerdDruck,5);

        zahl5 = S7_GetIntAt(MyDB100,44);

        S7_SetRealAt(MyDB101,2,p);  // Pitch
        S7_SetRealAt(MyDB101,6,v);  // Volume
        sf::Text VerdDruck ("VerdTemp: "+(Text_IW_VerdDruck)+" °C"+"\nX: "+to_string(position.x)+"\nY: "+to_string(position.y),font,25);


        std::string zeig = "Istwert Verdampfungsdruck: \t"+to_string(zahl1)+
        "\nIstwert Verflüssigungsdruck: \t"+to_string(zahl2)+
        "\nIstwert KaltwasserAustrittstemp: \t"+to_string(zahl3)+
        "\nIstwert KaltwasserEintrittstemp: \t"+to_string(zahl4)+
        "\nMaus_X: \t"+to_string(position.x)+
        "\nMaus_Y: \t"+to_string(position.y)+

        "\nAktuelle Stufenvorgabe an ZLT: \t"+to_string(zahl5);

        sf::Text text (zeig,font,25);

        sf::Time elapsed = Clock.restart();
        sf::Vector2i mouse = sf::Mouse::getPosition(app);
        particles.setEmitter(app.mapPixelToCoords(mouse));
        particles.update(elapsed);

        text.setPosition(50, 50);
        text.setFillColor(sf::Color(100,100,255,255));
        text.setOutlineColor(sf::Color(255,255,255,255));
        text.setOutlineThickness(2.0);

        sf::Vertex line1[] =
        {
            sf::Vertex(sf::Vector2f(position.x, 0)),
            sf::Vertex(sf::Vector2f(position.x, 1080))
        };

        sf::Vertex line2[] =
        {
            sf::Vertex(sf::Vector2f(0, position.y)),
            sf::Vertex(sf::Vector2f(1920, position.y))
        };

        sf::Vertex line3[] =
        {
            sf::Vertex(sf::Vector2f(0, 1080)),
            sf::Vertex(sf::Vector2f(position.x, position.y))
        };


        sf::Vertex line4[] =
        {
            sf::Vertex(sf::Vector2f(1920, 1080)),
            sf::Vertex(sf::Vector2f(position.x, position.y))
        };
        sf::Vertex line5[] =
        {
            sf::Vertex(sf::Vector2f(1920, 0)),
            sf::Vertex(sf::Vector2f(position.x, position.y))
        };
        sf::Vertex line6[] =
        {
            sf::Vertex(sf::Vector2f(0, 0)),
            sf::Vertex(sf::Vector2f(position.x, position.y))
        };
        VerdDruck.setOutlineColor(sf::Color(255,255,255,255));
        VerdDruck.setOutlineThickness(2.0);
        VerdDruck.setPosition(position.x,position.y);
        VerdDruck.setFillColor(sf::Color(100,100,255,255));

        music.setPitch(p);
        music.setVolume(v);
        sprite.setRotation(0);
        sprite.setOrigin(0, 0);
        sprite.setPosition(0, 0);
        sprite.setColor(sf::Color(255, 255, 255, 255));


        // Clear screen
        app.clear();
app.draw(Diagrammsprite);
        // Draw the sprite
      //  app.draw(sprite);
        app.draw(line1, 2, sf::Lines);
        app.draw(line2, 2, sf::Lines);
        app.draw(line3, 2, sf::Lines);
        app.draw(line4, 2, sf::Lines);
        app.draw(line5, 2, sf::Lines);
        app.draw(line6, 2, sf::Lines);

        app.draw(particles);
        app.draw(text);
        app.draw(VerdDruck);

        // Update the window
        app.display();
        ende = clock();
        diff = ende - start;
      //  cout << "benötigte Zeit: " << diff*1000/CLOCKS_PER_SEC << "\t" << position.x << "\t" << position.y << endl;

       // sf::sleep(sf::milliseconds(1000-(diff*1000/CLOCKS_PER_SEC)));
        zahl1++;
    }

    return EXIT_SUCCESS;
}
