package com.mvis.apps;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;

import java.util.HashMap;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.Utils.MSG_CLOSE_CONNECTION;


public class UsbStateReceiver extends BroadcastReceiver {
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    private static int VendorID = 0x148A;
    private static int ProductID = 0x0004;
    private static PendingIntent mPendingIntent;


    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();

        if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) {
            Log.i("UsbStateReceiver", "Usb attached");
            if (intent.getExtras().getBoolean("connected")) {
                Log.i("UsbStateReceiver", "Usb connected");
            } else {
                Log.i("UsbStateReceiver", "Usb disconnect");
                final UsbManager usbManager = (UsbManager) context.getSystemService(Context.USB_SERVICE);
                HashMap<String, UsbDevice> deviceList = usbManager.getDeviceList();
                if (!deviceList.isEmpty()) {
                    for (UsbDevice device : deviceList.values()) {
                        if ((device.getVendorId() == VendorID && device.getProductId() == ProductID)) {

                            if (!usbManager.hasPermission(device)) {
                                mPendingIntent = PendingIntent.getBroadcast(context, 0,
                                        new Intent(ACTION_USB_PERMISSION), 0);
                                usbManager.requestPermission(device, mPendingIntent);

                            } else {
                                Log.i("UsbStateReceiver", "We have the USB permission! ");
                            }
                        }
                    }
                }
            }
        } else if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
            Log.i("UsbStateReceiver", "Usb detached! ");
            Message msg0 = messageHandler.obtainMessage(MSG_CLOSE_CONNECTION);
            Bundle b = new Bundle();
            msg0.setData(b);
            messageHandler.sendMessage(msg0);
        }
    }
}
