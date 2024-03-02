package com.cara;

import androidx.core.content.FileProvider;
import android.content.Intent;
import android.net.Uri;
import java.io.File;
import android.content.Context;


public class ShareUtils {
    final static private String AUTHORITY = "com.cara.fileprovider";

    public static void share(String path, Context context) {
        File f = new File(path);
        Intent sendIntent = new Intent();
        sendIntent.setAction(Intent.ACTION_SEND);

        Uri uri = FileProvider.getUriForFile(context, AUTHORITY, f);
        sendIntent.putExtra(Intent.EXTRA_STREAM, uri);
        sendIntent.setType("application/pdf");

        Intent shareIntent = Intent.createChooser(sendIntent, "Compartir PDF");
        context.startActivity(shareIntent);
    }
}
