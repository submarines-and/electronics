import { Controller, Post, UploadedFile, UseInterceptors } from '@nestjs/common';
import { FileInterceptor } from '@nestjs/platform-express';
import { exec, execSync } from 'node:child_process';
import { log } from 'node:console';
import { unlinkSync, writeFileSync } from 'node:fs';
import { join } from 'node:path';

@Controller()
export class ImageProxyController {

  @Post('upload')
  @UseInterceptors(FileInterceptor('data'))
  public uploadFile(@UploadedFile() file: Express.Multer.File) {
    const heicFile = join(__dirname, 'tmp.heic');
    const bmpFile = join(__dirname, 'tmp.bmp')

    log(`Saving to disk: ${heicFile}`);
    writeFileSync(heicFile, file.buffer);

    log(`Converting to bmp...`);
    execSync(`magick ${heicFile} BMP3:${bmpFile}`);

    log(`Sending to photo frame in the background`);

    // IP to photo frame
    const photoFrameIp = process.env.photoFrameIp || '192.168.176.177';
    exec(`curl -v -F "data=@${bmpFile}" http://${photoFrameIp}/upload`, () => {
      log("Photo uploaded, cleaning up...");
      unlinkSync(heicFile);
      unlinkSync(bmpFile);
      log("Done!");
    });

    log(`Sending OK to client!`)
  }
}
