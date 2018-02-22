/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.example;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class ArrTest extends AppCompatActivity {
    static int[] arrays = {4,3,12,56,1,23,45,67};
    static String str[] = new String[] { "we", "are", "friends" };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /* Retrieve our TextView and set its content.
         * the text is retrieved by calling a native
         * function.
         */
        Hello.HelloTest(str);
        NativePeer.PeerTest(str);
        setContentView(R.layout.activity_hello_jni);
        TextView tv = (TextView)findViewById(R.id.hello_textview);
        tv.setText( "arrtest" );
        Father[] objArrays = {
                new Father(1, 10),
                new Father(2, 20),
                new Father(3, 30)
        };
    }

    public void onClick(View v){
        if(v.getId() == R.id.button) {
//            nativeMsg();
        }
    }


    static
    {
        System.loadLibrary("hello-jni");
    }
}
