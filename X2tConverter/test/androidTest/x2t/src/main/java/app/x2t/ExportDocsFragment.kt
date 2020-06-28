package app.x2t

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import kotlinx.android.synthetic.main.fragment_export_docs.*

class ExportDocsFragment constructor(): Fragment() {

    private var items: List<String> = emptyList()
    private lateinit var clickListener: (position: Int) -> Unit

    constructor(listener: (position: Int) -> Unit) : this() {
        clickListener = listener
    }

        override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        retainInstance = true
    }

    override fun onCreateView(inflater: LayoutInflater,
                              container: ViewGroup?,
                              savedInstanceState: Bundle?): View? =
            inflater.inflate(R.layout.fragment_export_docs, container, false)

    // populate the views now that the layout has been inflated
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        items = listOf(
                resources.getString(R.string.docx),
                resources.getString(R.string.odt),
                resources.getString(R.string.rtf),
                resources.getString(R.string.txt),
                resources.getString(R.string.dotx),
                resources.getString(R.string.ott)
        )
        // RecyclerView node initialized here
        list_recycler_view.apply {
            // set a LinearLayoutManager to handle Android
            // RecyclerView behavior
            layoutManager = LinearLayoutManager(activity)
            // set the custom adapter to the RecyclerView
            adapter = StringAdapter(items, context)
            setOnItemClickListener {
                clickListener.invoke(it)
            }
        }
    }

    inline fun RecyclerView.setOnItemClickListener(crossinline listener: (position: Int) -> Unit) {
        addOnItemTouchListener(RecyclerItemClickListener(this,
                object : RecyclerItemClickListener.OnItemClickListener {
                    override fun onItemClick(view: View, position: Int) {
                        listener(position)
                    }
                }))
    }

    companion object {
        fun newInstance(): ExportDocsFragment = ExportDocsFragment()
    }
}