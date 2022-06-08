package com.weather.stationtest;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

public class Temp_Hour_Adapter extends RecyclerView.Adapter<Temp_Hour_Adapter.MyViewHolder> {
    private List<Temp_Hour> tempList;
    class MyViewHolder extends RecyclerView.ViewHolder {
        TextView temp,hour;
        MyViewHolder(View view) {
            super(view);
            temp = view.findViewById(R.id.temp);
            hour = view.findViewById(R.id.hour);
        }
    }
    public Temp_Hour_Adapter(List<Temp_Hour> tempList) {
        this.tempList = tempList;
    }
    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.temp_hour, parent, false);
        return new MyViewHolder(itemView);
    }
    @Override
    public void onBindViewHolder(MyViewHolder holder, int position) {
        Temp_Hour temp_hour = tempList.get(position);
        holder.temp.setText(temp_hour.getTemp());
        holder.hour.setText(temp_hour.getHour());
    }
    @Override
    public int getItemCount() {
        return tempList.size();
    }
}
