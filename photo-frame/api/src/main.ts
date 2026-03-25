import { Module } from '@nestjs/common';
import { NestFactory } from '@nestjs/core';
import { ImageProxyController } from './image-proxy.controller';

@Module({
  imports: [],
  controllers: [ImageProxyController],
  providers: [],
})
export class ImageProxyModule { }

async function bootstrap() {
  const app = await NestFactory.create(ImageProxyModule);
  await app.listen(8080);
}
bootstrap();
