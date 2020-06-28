package app.x2t

import android.Manifest
import android.annotation.SuppressLint
import android.app.Activity
import android.content.Intent
import android.graphics.Rect
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
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
import java.io.File
import java.util.*


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
        const val OTT_EXTENSION = "ott"
        const val DOTX_EXTENSION = "dotx"
        const val XLTS_EXTENSION = "xlts"
        const val POTX_EXTENSION = "POTX"
        const val OTP_EXTENSION = "OTP"
        const val OTS_EXTENSION = "OTS"
    }

    private var docsFg: ExportDocsFragment? = null
    private var slidesFg: ExportSlidesFragment? = null
    private var cellsFg: ExportCellsFragment? = null
    private var isFragmentShown: Boolean = false
    private var documentType: String? = null
    private var resConvert: X2t.ConvertResult? = null
    private var builder = X2t.builder()
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
        exportButton.setOnClickListener(this)
        exportButton.isEnabled = false
        convertButton.isEnabled = false
        docsFg = ExportDocsFragment(::exportListener)
        slidesFg = ExportSlidesFragment(::exportListener)
        cellsFg = ExportCellsFragment(::exportListener)
        builder.setContext(applicationContext)
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
                            documentType = getConverterTypeTo()
                            convertButton.isEnabled = true
                            exportButton.isEnabled = false
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
            R.id.exportButton -> export()
        }
    }

    override fun dispatchTouchEvent(event: MotionEvent): Boolean {
        if (event.action == MotionEvent.ACTION_DOWN) {

            //and only is the ListFragment shown.

            //and only is the ListFragment shown.
            if (isFragmentShown) {
                // create a rect for storing the fragment window rect
                val r = Rect(0, 0, 0, 0)
                // retrieve the fragment's windows rect

                when(documentType)
                {
                    X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_WORD"] -> {
                        docsFg?.view?.getGlobalVisibleRect(r)
                    }

                    X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET"] -> {
                        cellsFg?.view?.getGlobalVisibleRect(r)
                    }

                    X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION"] -> {
                        slidesFg?.view?.getGlobalVisibleRect(r)
                    }
                }
                // check if the event position is inside the window rect
                val intersects = r.contains(event.x.toInt(), event.y.toInt())
                // if the event is not inside then we can close the fragment
                if (!intersects) {
                   // Log.d(docsFg.TAG, "pressed outside the listFragment")
                    hideExportFragment()

                    isFragmentShown = false
                    // notify that we consumed this event
                    return true
                }
            }
        }
        return super.dispatchTouchEvent(event)
    }

    @SuppressLint("MissingPermission")
    private fun converter() {
        builder.apply {
            getInputParams().apply {
                // тут инициализируюся фонты (их можно положить в core/X2tConverter/test/androidTest/x2t/src/main/assets)
                this@apply.fontsDir = mAssetsFontsDir
                this@apply.fontsJs = mAssetsFontsJs
                this@apply.to = null
                this@apply.temp = null
                this@apply.theme = null
                this@apply.xml = null
            }
        }
        builder.setConvertType(documentType)
        builder.setFromPath(mFilePath!!)
        builder.setIsClearTemp(false)

        logMessage("\nStart $documentType converting...")
        chooseButton.isEnabled = false
        convertButton.isEnabled = false
        resConvert?.apply { root?.apply{lib.x2t.utils.FileUtils.deletePath(this)} }

        converting()
    }
    @SuppressLint("MissingPermission")
    private fun converting()
    {
        mAsyncRoutines.run({ foreground, _ ->
            if (builder.convert().apply{resConvert = this}.isSuccess) {
                foreground.launch { logMessage("ConvertType PASS")
                    exportButton.isEnabled = true
                    convertButton.isEnabled = false}

            } else {
                foreground.launch { logMessage("\nConvertType FAILED") }
            }
        },
                {
                    showToast("converts PASSED!")
                    chooseButton.isEnabled = true
                    //convertButton.isEnabled = true
                },

                {
                    showToast("converts FAILED!")
                    chooseButton.isEnabled = true
                    //convertButton.isEnabled = true
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
                    TXT_EXTENSION, OTT_EXTENSION, DOTX_EXTENSION
                    -> X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_WORD"]

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

    @SuppressLint("MissingPermission")
    private fun export()
    {
        when(documentType)
        {
            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_WORD"] -> {
                supportFragmentManager.beginTransaction().replace(R.id.fragment_container, docsFg!!).commit()
                supportFragmentManager.beginTransaction().show(docsFg!!).commit()
                isFragmentShown = true
            }

            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET"] -> {
                supportFragmentManager.beginTransaction().replace(R.id.fragment_container, cellsFg!!).commit()
                supportFragmentManager.beginTransaction().show(cellsFg!!).commit()
                isFragmentShown = true
            }

            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION"] -> {
                supportFragmentManager.beginTransaction().replace(R.id.fragment_container, slidesFg!!).commit()
                supportFragmentManager.beginTransaction().show(slidesFg!!).commit()
                isFragmentShown = true
            }

            else -> return
        }

    }

    private fun exportListener(item: Int)
    {
        when(documentType)
        {
            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_WORD"] -> {
                when(item){
                    0 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(DOCX_EXTENSION)
                        })
                    }
                    1 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(ODT_EXTENSION)
                        })
                    }
                    2 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(RTF_EXTENSION)
                        })
                    }
                    3 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(TXT_EXTENSION)
                        })
                    }
                    4 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(DOTX_EXTENSION)
                        })
                    }
                    5 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(OTT_EXTENSION)
                        })
                    }
                }
                converting()
            }

            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET"] -> {
                when(item){
                    0 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(XLSX_EXTENSION)
                        })
                    }
                    1 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(ODS_EXTENSION)
                        })
                    }
                    2 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            delimiterCode = X2t.InputParams.DELIMITER_CODE_COMMA
                            to = getDefaultPathToExport(CSV_EXTENSION)
                        })
                    }
                    3 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTS"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(XLTS_EXTENSION)
                        })
                    }
                    4 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(OTS_EXTENSION)
                        })
                    }
                }
                converting()
            }

            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION"] -> {
                when(item){
                    0 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(PPTX_EXTENSION)
                        })
                    }
                    1 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(ODP_EXTENSION)
                        })
                    }
                    2 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(POTX_EXTENSION)
                        })
                    }
                    3 ->  builder.apply {
                        setConvertType(X2t.ConvertType["AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP"])
                        setIsClearTemp(true)
                        setIsOverwrite(true)
                        setInputParams(X2t.InputParams().apply {
                            from = resConvert!!.to!!
                            to = getDefaultPathToExport(OTP_EXTENSION)
                        })
                    }
                }
                converting()
            }

            else -> return
        }
        hideExportFragment()
    }

    private fun hideExportFragment()
    {
        when(documentType)
        {
            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_WORD"] -> {
                supportFragmentManager.beginTransaction().hide(docsFg!!).commit()
            }

            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET"] -> {
                supportFragmentManager.beginTransaction().hide(cellsFg!!).commit()
            }

            X2t.ConvertType["AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION"] -> {
                supportFragmentManager.beginTransaction().hide(slidesFg!!).commit()
            }
        }
    }
    private fun getDefaultPathToExport(extension: String): String {
        val name = (mFileName ?: "").substringBefore('.')
        var pathExport = "${resConvert!!.root}"
        var path = "${pathExport}/${name}.${extension.toLowerCase(Locale.getDefault())}"
        File(pathExport).let { if(!it.exists()) it.mkdirs() }
        val file = File(path)
        if (file.exists()) {
            val newName = (getNewFileName(file).name).substringBefore('.')
            path = "${pathExport}/${newName}.${extension.toLowerCase(Locale.getDefault())}"
        }
        return path
    }

    fun getNewFileName(file: File): File {
        var newFile: File = file
        var number = 0
        while (newFile.exists()) {
            number++
            newFile = if (file.isDirectory) {
                File(file.parent, file.name + "($number)")
            } else {
                val newFileName = StringBuilder(file.name)
                        .insert(file.name.indexOf('.'), "($number)")
                        .toString()
                File(file.parent, newFileName)
            }
        }
        return newFile
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
