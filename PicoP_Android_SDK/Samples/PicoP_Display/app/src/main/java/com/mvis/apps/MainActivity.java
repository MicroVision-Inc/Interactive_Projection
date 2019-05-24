package com.mvis.apps;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.Configuration;
import android.content.res.TypedArray;
import android.os.Bundle;
import android.os.Message;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;

import com.mvis.apps.adapter.NavDrawerListAdapter;
import com.mvis.apps.entity.NavDrawerItem;
import com.mvis.apps.fragment.*;
import com.mvis.apps.fragment.System;

import java.util.ArrayList;
import java.util.List;

import picop.interfaces.def.PicoP_ConnectionInfo;
import picop.interfaces.def.PicoP_Handle;
import picop.interfaces.def.PicoP_LibraryInfoS;

import static com.mvis.apps.MessageCenter.messageHandler;
import static com.mvis.apps.Utils.MSG_CLOSE_CONNECTION;
import static picop.interfaces.def.PicoP_ConnectionTypeE.*;

public class MainActivity extends FragmentActivity implements OnItemClickListener {

    public static boolean SERIAL_PORT_OPENED = false;
    /* { PicoP SDK code */
    public static PicoP_Handle mPicoPHandler;
    public static PicoP_LibraryInfoS mPicoPLibraryInfo;
    public static PicoP_ConnectionInfo connectionInfo;
    /* PicoP SDK code }*/
    public static Context mContext;
    private CharSequence mTitle;
    private CharSequence mDrawerTitle;
    private String[] mNavMenuTitles;
    private DrawerLayout mDrawerLayout;
    private ListView mDrawerList;
    private List<NavDrawerItem> mNavDrawerItems;
    private TypedArray mNavMenuIconsTypeArray;
    private NavDrawerListAdapter mAdapter;
    private ActionBarDrawerToggle mDrawerToggle;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        setContentView(R.layout.activity_main);

        mPicoPLibraryInfo = new PicoP_LibraryInfoS();
        mContext = this.getApplication();

        // For USB port
        mPicoPHandler = new PicoP_Handle(eUSB);
        connectionInfo = mPicoPHandler.getConnectInfo();
        connectionInfo.setConnectionContext(mContext);

        findView();
        if (savedInstanceState == null) {
            selectItem(0);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Message msg0 = messageHandler.obtainMessage(MSG_CLOSE_CONNECTION);
        Bundle b = new Bundle();
        msg0.setData(b);
        messageHandler.sendMessage(msg0);
    }

    @SuppressLint("NewApi")
    private void findView() {

        Log.i("MainActivity", "findView() enter!");

        mTitle = mDrawerTitle = getTitle();

        mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        mDrawerList = (ListView) findViewById(R.id.left_drawer);

        // set a custom shadow that overlays the main content when the drawer opens
        mDrawerLayout.setDrawerShadow(R.drawable.drawer_shadow, GravityCompat.START);

        mNavMenuTitles = getResources().getStringArray(R.array.nav_drawer_items);
        // nav drawer icons from resources
        mNavMenuIconsTypeArray = getResources()
                .obtainTypedArray(R.array.nav_drawer_icons);

        mNavDrawerItems = new ArrayList<NavDrawerItem>();

        // adding nav drawer items to array
        // Home
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[0], mNavMenuIconsTypeArray
                .getResourceId(0, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[1], mNavMenuIconsTypeArray
                .getResourceId(1, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[2], mNavMenuIconsTypeArray
                .getResourceId(2, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[3], mNavMenuIconsTypeArray
                .getResourceId(3, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[4], mNavMenuIconsTypeArray
                .getResourceId(4, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[5], mNavMenuIconsTypeArray
                .getResourceId(5, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[6], mNavMenuIconsTypeArray
                .getResourceId(6, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[7], mNavMenuIconsTypeArray
                .getResourceId(7, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[8], mNavMenuIconsTypeArray
                .getResourceId(8, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[9], mNavMenuIconsTypeArray
                .getResourceId(9, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[10], mNavMenuIconsTypeArray
                .getResourceId(10, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[11], mNavMenuIconsTypeArray
                .getResourceId(11, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[12], mNavMenuIconsTypeArray
                .getResourceId(12, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[13], mNavMenuIconsTypeArray
                .getResourceId(13, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[14], mNavMenuIconsTypeArray
                .getResourceId(14, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[15], mNavMenuIconsTypeArray
                .getResourceId(15, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[16], mNavMenuIconsTypeArray
                .getResourceId(16, -1)));
        mNavDrawerItems.add(new NavDrawerItem(mNavMenuTitles[17], mNavMenuIconsTypeArray
                .getResourceId(17, -1)));
        // Recycle the typed array
        mNavMenuIconsTypeArray.recycle();

        // setting the nav drawer list adapter
        mAdapter = new NavDrawerListAdapter(getApplicationContext(), mNavDrawerItems);
        mDrawerList.setAdapter(mAdapter);
        mDrawerList.setOnItemClickListener(this);


        // enable ActionBar app icon to behave as action to toggle nav drawer
        getActionBar().setDisplayHomeAsUpEnabled(true);
        getActionBar().setHomeButtonEnabled(true);
        getActionBar().setHomeAsUpIndicator(R.drawable.ic_drawer);

        // ActionBarDrawerToggle ties together the the proper interactions
        // between the sliding drawer and the action bar app icon
        mDrawerToggle = new ActionBarDrawerToggle(
                this,                  /* host Activity */
                mDrawerLayout,         /* DrawerLayout object */
                //R.drawable.ic_drawer,  /* nav drawer image to replace 'Up' caret */
                R.string.drawe_open,  /* "open drawer" description for accessibility */
                R.string.drawe_close  /* "close drawer" description for accessibility */
        ) {
            public void onDrawerClosed(View view) {
                getActionBar().setTitle(mTitle);
                invalidateOptionsMenu(); // creates call to onPrepareOptionsMenu()
                mDrawerToggle.syncState();
            }

            public void onDrawerOpened(View drawerView) {
                getActionBar().setTitle(mDrawerTitle);
                invalidateOptionsMenu(); // creates call to onPrepareOptionsMenu()
                mDrawerToggle.syncState();
            }
        };
        mDrawerLayout.addDrawerListener(mDrawerToggle);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // The action bar home/up action should open or close the drawer.
        // ActionBarDrawerToggle will take care of this.
        if (mDrawerToggle.onOptionsItemSelected(item)) {
            return true;
        }
        // Handle action buttons
        switch (item.getItemId()) {
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        // Sync the toggle state after onRestoreInstanceState has occurred.
        mDrawerToggle.syncState();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        // Pass any configuration change to the drawer toggls
        mDrawerToggle.onConfigurationChanged(newConfig);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position,
                            long id) {

        selectItem(position);
    }

    /**
     * Diplaying fragment view for selected nav drawer list item
     */
    private void selectItem(int position) {
        // update the main content by replacing fragments
        Fragment fragment = null;
        switch (position) {
            case 0:
                fragment = new HomeFragment();
                break;
            case 1:
                fragment = new Phase();
                break;
            case 2:
                fragment = new Brightness();
                break;
            case 3:
                fragment = new ColorAlignment();
                break;
            case 4:
                fragment = new ColorConvertor();
                break;
            case 5:
                fragment = new ColorMode();
                break;
            case 6:
                fragment = new DrawPattern();
                break;
            case 7:
                fragment = new EventLog();
                break;
            case 8:
                fragment = new FlipState();
                break;
            case 9:
                fragment = new FrameRate();
                break;
            case 10:
                fragment = new GammaBoost();
                break;
            case 11:
                fragment = new GammaVal();
                break;
            case 12:
                fragment = new InputVideoState();
                break;
            case 13:
                fragment = new InputVideoProperties();
                break;
            case 14:
                fragment = new MicroweaveMode();
                break;
            case 15:
                fragment = new OutputVideoState();
                break;
            case 16:
                fragment = new OutputVideoProperties();
                break;
            case 17:
                fragment = new SensingState();
                break;
            case 18:
                fragment = new System();
                break;
            default:
                break;
        }

        if (fragment != null) {
            FragmentManager fragmentManager = getSupportFragmentManager();
            fragmentManager.beginTransaction()
                    .replace(R.id.content_frame, fragment).commit();

            // update selected item and title, then close the drawer
            mDrawerList.setItemChecked(position, true);
            mDrawerList.setSelection(position);
            setTitle(mNavMenuTitles[position]);
            mDrawerLayout.closeDrawer(mDrawerList);
        } else {
            // error in creating fragment
            Log.e("MainActivity", "Error in creating fragment");
        }
    }

    @SuppressLint("NewApi")
    @Override
    public void setTitle(CharSequence title) {
        mTitle = title;
        getActionBar().setTitle(mTitle);
    }

}
