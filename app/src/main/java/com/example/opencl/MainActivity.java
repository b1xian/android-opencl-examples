package com.example.opencl;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private Button button_info;
    private Button button_vec_add;
    private Button button_conv;

    private TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        button_info = findViewById(R.id.bt_info);
        button_vec_add = findViewById(R.id.bt_vec_add);
        button_conv = findViewById(R.id.bt_conv);
        textView = findViewById(R.id.text_view);

        button_info.setOnClickListener(this);
        button_vec_add.setOnClickListener(this);
        button_conv.setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.bt_info:
                onClickBtnInfo(view);
                break;
            case R.id.bt_vec_add:
                onClickBtnVecAdd(view);
                break;
            case R.id.bt_conv:
                onClickBtnConv(view);
                break;
            default:
                break;
        }
    }

    private void onClickBtnInfo(View view) {
        String info = Introduce.showInfo();
        textView.setText(info);
    }

    private void onClickBtnVecAdd(View view) {
        Introduce.vecAdd();
    }

    private void onClickBtnConv(View view) {
        Introduce.conv();
    }


}
