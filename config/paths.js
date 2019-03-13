const path = require('path')
const fs = require('fs')

const appDirectory = fs.realpathSync(process.cwd())
const resolveApp = relativePath => path.resolve(appDirectory, relativePath)

module.exports = {
  appPath: resolveApp('.'),
  appBuild: resolveApp('build'),
  appPublic: resolveApp('public'),
  appHtml: resolveApp('public/index.html'),
  appEntry: resolveApp('src/EntryMain.bs.js'),

  appArticleHtml: resolveApp('public/article.html'),
  appArticleEntry: resolveApp('src/EntryArticle.bs.js'),

  appCategoryHtml: resolveApp('public/category.html'),
  appCategoryEntry: resolveApp('src/EntryCategory.bs.js'),

  appWechatHtml: resolveApp('public/wechat.html'),
  appWechatEntry: resolveApp('src/EntryWechat.bs.js'),
}
