package com.example.gsmtvremote;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.os.Build;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;
import androidx.core.content.ContextCompat;

public class NotificationHelper {

    private static final String CHANNEL_ID = "your_channel_id"; // Change this to a unique identifier for your channel

    private final Context context;

    public NotificationHelper(Context context) {
        this.context = context;
        createNotificationChannel();
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name = "LifeLink"; // Change this to the desired channel name
            String description = "Emergency notifications from LifeLink Client"; // Change this to the desired channel description
            int importance = NotificationManager.IMPORTANCE_DEFAULT;

            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);

            // Register the channel with the system
            NotificationManager notificationManager = context.getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }

    public void showNotification(String title, String content) {
        NotificationCompat.Builder builder = new NotificationCompat.Builder(context, CHANNEL_ID)
                .setSmallIcon(android.R.drawable.ic_dialog_info)
                .setContentTitle(title)
                .setContentText(content)
                .setPriority(NotificationCompat.PRIORITY_HIGH)
                .setColor(ContextCompat.getColor(context, R.color.colorEmergencyNotification)) // Replace R.color.your_red_color with the resource ID of your red color
                .setColorized(true);

        // Get the NotificationManager
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);

        // Generate a unique notification ID (you may use a constant or dynamic ID)
        int notificationId = (int) System.currentTimeMillis();

        // Build and display the notification
        notificationManager.notify(notificationId, builder.build());
    }

    public void cancelNotification(int notificationId) {
        // Get the NotificationManager
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);

        // Cancel the notification by its ID
        notificationManager.cancel(notificationId);
    }
}
