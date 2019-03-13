const path = require('path')
const webpack = require('webpack')
const HtmlWebpackPlugin = require('html-webpack-plugin')
const CleanWebpackPlugin = require('clean-webpack-plugin')
const UglifyWebpackPlugin = require('uglifyjs-webpack-plugin')
const paths = require('./paths')

const isProd = process.env.NODE_ENV === 'production'

const entries = {
  index: [paths.appEntry, paths.appHtml],
  article: [paths.appArticleEntry, paths.appArticleHtml],
  category: [paths.appCategoryEntry, paths.appCategoryHtml],
  wechat: [paths.appWechatEntry, paths.appWechatHtml],
}

const entryKeys = Object.keys(entries)
const htmlPlugins = entryKeys.map(key => {
  const otherEntryKeys = entryKeys.filter(k => k !== key)

  return new HtmlWebpackPlugin({
    excludeChunks: otherEntryKeys.reduce(
      (r, k) => [...r, k, `runtime~${k}`],
      []
    ),
    template: entries[key][1],
    filename: `${key}.html`,
  })
})

const CDN_URL = 'https://img0.whenpigsfly.cn/wx/'

module.exports = {
  mode: isProd ? 'production' : 'development',

  entry: Object.keys(entries).reduce(
    (r, k) => ({ ...r, [k]: entries[k][0] }),
    {}
  ),

  devtool: isProd ? 'source-map' : 'cheap-module-source-map',

  output: {
    path: isProd ? paths.appBuild : undefined,
    pathinfo: true,
    publicPath: isProd ? CDN_URL : '/',
    filename: isProd ? 'js/[name].[contenthash:8].js' : 'js/[name].js',
    // chunkFilename: 'js/[name].chunk.js',
  },

  optimization: {
    splitChunks: {
      chunks: 'all',
      name: true,
    },
    runtimeChunk: true,
    minimizer: [
      isProd
        ? new UglifyWebpackPlugin({
            cache: true,
            parallel: true,
            uglifyOptions: {
              output: {
                comments: /@license/i,
              },
            },
          })
        : null,
    ].filter(Boolean),
  },

  module: {
    rules: [
      {
        test: /\.styl$/,
        use: ['style-loader', 'css-loader', 'stylus-loader'],
      },
    ],
  },

  resolve: {
    modules: [path.resolve(__dirname, '../src'), 'node_modules'],
  },

  plugins: [
    isProd
      ? new CleanWebpackPlugin(paths.appBuild, {
          root: path.resolve(__dirname, '..'),
        })
      : null,

    ...htmlPlugins,

    new webpack.DefinePlugin({
      'process.env': {
        isDev: process.env.NODE_ENV !== 'production',
        LC_APP_ID: process.env.LC_APP_ID,
        LC_APP_KEY: process.env.LC_APP_KEY,
      },
    }),
  ].filter(Boolean),

  devServer: {
    compress: true,
    contentBase: paths.appPublic,
    watchContentBase: true,
    port: process.env.PORT || 9000,
  },
}
