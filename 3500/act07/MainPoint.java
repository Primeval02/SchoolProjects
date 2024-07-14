//CS 3500 - Class Point
//Works with p\Point.java
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;
import java.util.Arrays;

public class MainPoint {
    public static void main(String[] args) {
        //creating a new oject point with corrdinates (0, 0)
        Point origin = new Point();

        //creating mote object points
        Point p1 = new Point(1, 2);
        Point p2 = new Point(3, 4);
        Point p3 = new Point(3, 4);
        Point p4 = p1;
        double d12;

        //printing all points
        System.out.println("\nObjects (Poins) created from Point class:");
        System.out.println("p1: " + p1);
        System.out.println("p2: " + p2);
        System.out.println("p3: " + p3);
        System.out.println("p4: " + p4);

        //comparing some points
        System.out.println("\nComparing Points:");
        System.out.println("p1 == p1? " + (p1 == p1));
        System.out.println("p1 == p2? " + (p1 == p2));
        System.out.println("p2 == p3? " + (p2 == p3));
        System.out.println("p1 == p4? " + (p1 == p4));

        //caluclatin the distance between p1 and p2
        d12 = p1.distance(p2);
        System.out.println("\nThe distance between p1 and p2 is: " + d12 + "\n");

        // Setting new coordinates on point 1
        System.out.println("\nSetting new coordinates for point 1: ");
        p1.setX(-99);
        p1.setY(-1);
        System.out.println("p1: " + p1);

        System.out.println("\nChanging some values at the object level:");

        // changing the value of x of p1 changes it at the object level.
        // p4 refers to the same object so printing p4 will see the new
        // value too.
        p1.setX(5);

        // Setting p1 equal to a new Point only changes what p1 points too.
        // p4 still points to the original Point object.
        p1 = new Point(7, 8);

        System.out.println("p1: " + p1);
        System.out.println("p4: " + p4);

        System.out.println("p1.equals(p1)? " + p1.equals(p1));
        System.out.println("p1.equals(p2)? " + p1.equals(p2));
        System.out.println("p2.equals(p3)? " + p2.equals(p3));
        System.out.println("p1.equals(p4)? " + p1.equals(p4));



        BufferedReader reader;
        try {
            reader = new BufferedReader(new FileReader("/home/stu/btate/3500/act07/inputpoint.txt"));
            Point[] objects = new Point[1000];
            String line = reader.readLine();
            String[] splitStr = line.split("\\s+");
            int x = Integer.parseInt(splitStr[0]);
            int y = Integer.parseInt(splitStr[1]);

            do{
                //Creates points 
                for (int i = 0; i < 1000; i++) {
                    objects[i] = new Point(i, x, y);
                    line = reader.readLine();
                    if (line != null) {
                        splitStr = line.split("\\s+");
                        x = Integer.parseInt(splitStr[0]);
                        y = Integer.parseInt(splitStr[1]);
                    }
                }
            }while (line != null);

            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }



    }
}
