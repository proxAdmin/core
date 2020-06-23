package app.x2t

import android.Manifest
import android.annotation.SuppressLint
import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.annotation.RequiresPermission
import androidx.appcompat.app.AppCompatActivity
import app.x2t.utils.AsyncRoutines
import app.x2t.utils.FileUtils
import app.x2t.utils.PathUtils
import app.x2t.utils.PermissionUtils
import kotlinx.android.synthetic.main.activity_converter.*
import kotlinx.coroutines.launch
import lib.x2t.X2t

class ConverterActivity : AppCompatActivity(), View.OnClickListener {

    companion object {
        val TAG = ConverterActivity::class.java.simpleName
        private const val REQUEST_CODE_DOC = 1
        private const val CONVERTER_PREFIX = "converter_"
        private const val PERMISSION_READ_WRITE_STORAGE = 1
        private const val PERMISSION_CHOOSER_READ_STORAGE = 2

        const val DOCX_EXTENSION = "docx"
        const val DOC_EXTENSION = "doc"
        const val ODT_EXTENSION = "odt"
        const val TXT_EXTENSION = "txt"
        const val RTF_EXTENSION = "rtf"
        const val PPTX_EXTENSION = "pptx"
        const val PPT_EXTENSION = "ppt"
        const val ODP_EXTENSION = "odp"
        const val XLSX_EXTENSION = "xlsx"
        const val XLS_EXTENSION = "xls"
        const val ODS_EXTENSION = "ods"
        const val CSV_EXTENSION = "csv"
        const val PDF_EXTENSION = "pdf"
    }

    private val mToast: Toast by lazy {
        Toast.makeText(this, "", Toast.LENGTH_LONG)
    }

    private val mAsyncRoutines = AsyncRoutines()
    private var mFilePath: String? = null
    private var mFileName: String? = null
    private var mFolderPath: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_converter)
        convertButton.setOnClickListener(this)
        chooseButton.setOnClickListener(this)
    }

    @SuppressLint("MissingPermission")
    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        when (requestCode) {
            PERMISSION_READ_WRITE_STORAGE -> {
                if (PermissionUtils.checkReadWritePermission(this)) {
                    converter()
                }
            }

            PERMISSION_CHOOSER_READ_STORAGE -> {
                if (PermissionUtils.checkReadPermission(this)) {
                    browseDocuments()
                }
            }
        }
    }

    public override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (resultCode == Activity.RESULT_OK) {
            when (requestCode) {
                REQUEST_CODE_DOC -> {
                    if (data != null && data.data != null) {
                        mFilePath = PathUtils.getPath(this, data.data!!)

                        if (mFilePath != null) {
                            mFolderPath = mFilePath!!.substring(0, mFilePath!!.lastIndexOf("/"))
                            mFileName = mFilePath!!.replace(mFolderPath!! + "/", "")
                            logMessage("\n-------------------------------------------------")
                            logMessage("Source file path: " + mFilePath!!)
                            logMessage("Source file name: " + mFileName!!)
                            logMessage("Source folder path: " + mFolderPath!!)
                        } else {
                            showToast("UNKNOWN ERROR!")
                            logMessage("\nUNKNOWN ERROR!\n")
                        }
                    }
                }
            }
        }
    }

    @SuppressLint("MissingPermission")
    override fun onClick(view: View) {
        when (view.id) {
            R.id.convertButton -> converter()
            R.id.chooseButton -> browseDocuments()
        }
    }

    @SuppressLint("MissingPermission")
    private fun converter() {
        val builder = X2t.builder().setContext(applicationContext).apply {
            getInputParams().apply {
                // тут инициализируюся фонты (их можно положить в core/X2tConverter/test/androidTest/x2t/src/main/assets)
                this@apply.fontsDir = mAssetsFontsDir
                this@apply.fontsJs = mAssetsFontsJs
            }
        }
        builder.setConvertType(getConverterTypeTo())
        builder.setFromPath(mFilePath!!)

        logMessage("\nStart " + getConverterTypeTo() + " converting...")
        chooseButton.isEnabled = false
        convertButton.isEnabled = false

        mAsyncRoutines.run({ foreground, instance ->
            if (builder.convert()?.isSuccess) {
                foreground.launch { logMessage("ConvertType PASS") }

            } else {
                foreground.launch { logMessage("\nConvertType FAILED") }
            }
        },
                {
                    showToast("converts PASSED!")
                    chooseButton.isEnabled = true
                    convertButton.isEnabled = true
                },

                {
                    showToast("converts FAILED!")
                    chooseButton.isEnabled = true
                    convertButton.isEnabled = true
                }
        )

    }

    @SuppressLint("MissingPermission")
    private fun getConverterTypeTo(): String? {
        if (PermissionUtils.requestReadWritePermission(this, PERMISSION_READ_WRITE_STORAGE)) {
            if (mFilePath != null && mFileName != null && mFolderPath != null) {
                return when (FileUtils.getExtension(mFilePath!!).toLowerCase()) {

                    DOCX_EXTENSION, DOC_EXTENSION,
                    ODT_EXTENSION, RTF_EXTENSION,
                    TXT_EXTENSION -> X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_WORD"]

                    XLSX_EXTENSION, XLS_EXTENSION,
                    ODS_EXTENSION, CSV_EXTENSION
                    -> X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET"]

                    PPTX_EXTENSION, PPT_EXTENSION,
                    ODP_EXTENSION
                    -> X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION"]

                    else -> {
                        showToast("Unsupported format!")
                        null
                    }
                }
            }
        }
        showToast("Unsupported format!")
        return null
    }

    private fun showToast(message: String) {
        mToast.setText(message)
        mToast.show()
    }

    private fun logMessage(message: String) {
        Log.d(TAG, message)
        val stringBuffer = StringBuffer()
        stringBuffer.append(loggerText.text)
        stringBuffer.append("\n").append(message)
        loggerText.text = stringBuffer.toString()
        scrollView.scrollTo(0, loggerText.bottom)
    }

    @RequiresPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
    private fun browseDocuments() {
        if (PermissionUtils.requestReadPermission(this, PERMISSION_CHOOSER_READ_STORAGE)) {
            val intent = Intent(Intent.ACTION_GET_CONTENT)
            intent.type = "*/*"
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
            startActivityForResult(Intent.createChooser(intent, getString(R.string.button_choose)), REQUEST_CODE_DOC)
        }
    }

    protected val mCachePath: String by lazy {
        lib.x2t.utils.FileUtils.getCachePath(applicationContext)
    }

    protected val mAssetsFontsDir: String by lazy {
        "$mCachePath/fonts_js"
    }

    protected val mAssetsFontsJs: String by lazy {
        "$mCachePath/assets/fonts"
    }

}
