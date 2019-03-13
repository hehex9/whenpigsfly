/* eslint-disable no-console */
const { spawn } = require('child_process')
const fs = require('fs')
const path = require('path')
const qiniu = require('qiniu')

require('dotenv').config()
const { QINIU_ACCESS_KEY: accessKey, QINIU_SECRET_KEY: secretKey } = process.env

const log = console.log.bind(console)

function getToken() {
  const mac = new qiniu.auth.digest.Mac(accessKey, secretKey)
  const putPolicy = new qiniu.rs.PutPolicy({ scope: 'wpf-img0' })
  return putPolicy.uploadToken(mac)
}

function upload(config, uploadToken, filePath, filename) {
  const formUploader = new qiniu.form_up.FormUploader(config)
  const putExtra = new qiniu.form_up.PutExtra()

  return new Promise(resolve => {
    formUploader.putFile(
      uploadToken,
      `wx/js/${filename}`,
      filePath,
      putExtra,
      (e, body, info) => {
        if (e) {
          resolve(false)
        }

        if (info.statusCode == 200) {
          resolve(true)
        } else {
          resolve(false)
        }
      }
    )
  })
}

function cli(command, envs = null, options = null) {
  const [cmd, ...args] = command
  const env = Object.create(process.env)
  Object.assign(env, envs)

  return new Promise((resolve, reject) => {
    const runner = spawn(cmd, args, {
      env,
      stdio: 'inherit',
      ...options,
    })

    runner.on('exit', () => {
      resolve()
    })

    runner.on('error', err => {
      reject(err)
    })
  })
}

function build() {
  return cli(
    ['./node_modules/.bin/webpack', '--config', 'config/webpack.config.js'],
    { NODE_ENV: 'production' }
  )
}

function makeWorld() {
  return cli(['yarn', 'run', 'build:bsb'])
}

async function uploadStaticFiles() {
  const jsPath = path.join(__dirname, '../build', 'js')
  const files = fs.readdirSync(jsPath).filter(i => i.endsWith('.js'))

  const uploadToken = getToken()
  const config = new qiniu.conf.Config()
  config.zone = qiniu.zone.Zone_z0

  const result = await Promise.all(
    files.map(f => upload(config, uploadToken, path.join(jsPath, f), f))
  )
  const successCount = result.filter(Boolean).length
  log(`Done. uploaded (${successCount}/${files.length}) files`)
}

function syncHTMLFiles() {
  return cli(
    [
      'scp',
      '-r',
      path.join(__dirname, '../build/*.html'),
      process.env.WEB_SERVER,
    ],
    null,
    { shell: true }
  )
}

makeWorld()
  .then(() => {
    log('building...')
    return build()
  })
  .then(() => {
    log('uploading js files...')
    return uploadStaticFiles()
  })
  .then(() => {
    log()
    log('syncing html files...')
    return syncHTMLFiles()
  })
  .catch(e => {
    log('something wrong happend.')
    log(e)
  })
