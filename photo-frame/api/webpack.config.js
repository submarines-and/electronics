/**
 *
 * This is the nest webpack file.
 * It exists to allow bundling nest apps into a single main.js bundle.
 *
 */

/**
 * These are known lazy imports in various packages that we do not have.
 * They can safely be ignored in a build output.
 */
const lazyImports = [
];

// eslint-disable-next-line @typescript-eslint/no-require-imports, no-undef
const webpack = require('webpack');

// eslint-disable-next-line no-undef
module.exports = function (options) {
  const { plugins, ...config } = options;

  return {
    ...config,
    externals: [],
    plugins: [
      ...plugins,
      new webpack.IgnorePlugin({
        checkResource(resource) {

          if (!lazyImports.includes(resource)) {
            return false;
          }

          try {
            // eslint-disable-next-line no-undef
            require.resolve(resource, { paths: [process.cwd()] });
          }
          catch (err) {
            return true;
          }

          return false;
        },
      }),
      new webpack.optimize.LimitChunkCountPlugin({ maxChunks: 1 }),
    ],
  };
};
