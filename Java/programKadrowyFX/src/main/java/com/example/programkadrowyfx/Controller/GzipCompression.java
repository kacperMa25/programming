package com.example.programkadrowyfx.Controller;

import java.io.*;
import java.util.zip.GZIPOutputStream;
import java.util.zip.GZIPInputStream;

public class GzipCompression {

    public static void compressToGzip(String sourceFile, String gzipFile) throws IOException {
        try (FileInputStream fis = new FileInputStream(sourceFile);
             FileOutputStream fos = new FileOutputStream(gzipFile);
             GZIPOutputStream gzos = new GZIPOutputStream(fos)) {

            byte[] buffer = new byte[1024];
            int length;
            while ((length = fis.read(buffer)) >= 0) {
                gzos.write(buffer, 0, length);
            }
        }
    }

    public static void decompressGzip(String gzipFile, String outputFile) throws IOException {
        try (FileInputStream fis = new FileInputStream(gzipFile);
             GZIPInputStream gzis = new GZIPInputStream(fis);
             FileOutputStream fos = new FileOutputStream(outputFile)) {

            byte[] buffer = new byte[1024];
            int length;
            while ((length = gzis.read(buffer)) >= 0) {
                fos.write(buffer, 0, length);
            }
        }
    }
}
