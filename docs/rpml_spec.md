# RPML Specification [WIP]

### module
```xml
import yuki.rpml.core;
import yuki.rpml.ui;
<Window>

</Window>
```
### Button
```xml
import yuki.rpml.core;
import yuki.rpml.ui;

export Button {
  enum State {
    Normal,
    Hover,
    MouseDown,
  } State;

  string Text;

  Size sizeHint {
  
  };

  Brush BorderBrush;
  Brush Background;
  Brush Foreground;

  event Clicked;

  @render {
    return (
      @State::Normal
      <Rectangle StrokeWidth=1
                 StrokeBrush={BorderBrush}
                 Fill={Background}>
        <Text Content={Text} Brush={Foreground}/>
      </Rectangle>
      @State::Hover
      
      @State::MouseDown

    )
  }

  @mouse.down {

  }

  @mouse.hover {
    State = State::Hover;
  }

  @mouse.up {
    raise Clicked;
  }

  @key.up.enter {
  
  }
}
```
### TextBox
```xml
import yuki.rpml.core;
import yuki.rpml.ui;

export TextBox {
  string Content;
  string PlaceHolder;
  
  Font Font;

  @render {
    return (
      @State::Normal
      <Rectangle StrokeWidth=1
                 StrokeBrush={BorderBrush}
                 Fill={Background}>
        <Text name="textArea" Content={Content} Brush={Foreground}/>
      </Rectangle>
      @State::Hover
      
      @State::MouseDown

      @Content.isEmpty()
      .textArea {
        Content: {PlaceHolder}
      }

    )
  }

  @mouse.hover(#textArea) {
    $mouse.State = Editing;
  }

  @key.up {
    notice Content;
  }
}
```

```xml
import yuki.rpml.core;
import yuki.rpml.ui;

export ListView {
  ListViewItem Items;

  @render {
    return (
    )
  }
}
```

// [xx, xx] = ss;

```xml
import yuki.rpml.core;
import yuki.rpml.ui;

ScrollBar {

}

export ScrollView {
  View Content;
  float ScrollPositionX;
  float ScrollPositionY;

  @render {
    return (
    )
  }

  @mouse.scroll {

  }
}
```

### GridPane
```xml
import yuki.rpml.core;
import yuki.rpml.ui;

ScrollBar {

}

export ScrollView {
  View Content;
  float ScrollPositionX;
  float ScrollPositionY;

  @render {
    return (
    )
  }

  @mouse.scroll {

  }
}
```

```xml
<Window>

</Window>


```
