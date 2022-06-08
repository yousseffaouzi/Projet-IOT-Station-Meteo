package com.weather.stationtest;

public class Temp_Hour {
    private int hour, temp;
    public Temp_Hour(int temp, int hour){
        this.hour = hour;
        this.temp = temp;
    }

    public void setHour(int hour) {
        this.hour = hour;
    }

    public int getHour() {
        return hour;
    }

    public int getTemp() {
        return temp;
    }
}
