import processing.serial.*;
import java.util.Scanner;

//make Serial object arduino for communicate with arduino
Serial arduino;

//make Sector 2D array object maze for displaying maze
Sector[][] maze = new Sector[9][9];

//define width and height of one sector
int width = 80;
int height = 80;
int x, y;

Scanner input = new Scanner(System.in);

//setup
void setup() {
  //set size to 1000x1000
  size(1000, 1000);  
  
  //for loop for initialize 2D array maze
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      //constructor
      maze[y][x] = new Sector(x * width, y * height, width, height);
      //set color to grey
      maze[y][x].setColor(150);
    }
  }
  
  //print available serial ports
  printArray(Serial.list());
  byte port = Byte.parseByte(input.nextLine());
  try {
    arduino = new Serial(this, Serial.list()[port], 9600);
  } catch(ArrayIndexOutOfBoundsException e) {
    println("This serial port doesn't exist");
    return;
  }
}

void draw() {
  for (Sector[] x : maze) {
    for (Sector y : x) {
      y.render();
    }
  }
  
  String message = "4:0\n1:1:1\n-180";
  while (arduino.available() > 0) {
    message = arduino.readString();
  }
  String splittedMessage[] = message.split("\n");
  String xy[] = splittedMessage[0].split(":");
  String walls[] = splittedMessage[1].split(":");
  x = Integer.parseInt(xy[0]);
  y = Integer.parseInt(xy[1]);
  int heading = Integer.parseInt(splittedMessage[2]);
  
  maze[y][x].setColor(color(0, 206, 182));
  
  x *= 80;
  y *= 80;
    
  if (walls[0].equals("1")) {
    drawWall(heading, 'L');
  }
  if (walls[1].equals("1")) {
    drawWall(heading, 'S');
  }
  if (walls[2].equals("1")) {
    drawWall(heading, 'R');
  }
}

void drawWall(int heading, char direction) {
  stroke(0);
  if (direction == 'L') {
    if (heading > -20 && heading < 20) {
      line(x, y, x + width, y);
    } else if (heading > 20 && heading < 110) {
      line(x + width, y, x + width, y + height);
    } else if (heading > 110 && heading <= 180 || heading < -110 && heading >= -180) {
      line(x, y + height, x + width, y + height);
    } else if (heading < -20 && heading > 110) {
      line(x, y, x, y + height);
    }
  } else if (direction == 'S') {
    if (heading > -20 && heading < 20) {
      line(x + width, y, x + width, y + height);
    } else if (heading > 20 && heading < 110) {
      line(x, y + height, x + width, y + height);
      println("here");
    } else if (heading > 110 && heading <= 180 || heading < -110 && heading >= -180) {
      line(x, y, x, y + height);
    } else if (heading < -20 && heading > 110) {
      line(x, y, x + width, y);
    }
  } else if (direction == 'R') {
    if (heading > -20 && heading < 20) {
      line(x, y + height, x + width, y + height);
    } else if (heading > 20 && heading < 110) {
      line(x, y, x, y + height);
    } else if (heading > 110 && heading <= 180 || heading < -110 && heading >= -180) {
      line(x, y, x + width, y);
    } else if (heading < -20 && heading > 110) {
      line(x + width, y, x + width, y + height);
    }
  }
}
